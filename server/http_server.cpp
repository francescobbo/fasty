#include "http_server.h"
#include "request_line.h"
#include "../utils/http_status_codes.h"

#include <iostream>
using namespace std;

void HttpServer::InitThread(ClientSocket *client) {
	try {
		HttpServer s(*client);
		s.run();
	} catch (std::exception &e) {
		cout << "Connection aborted." << endl;
		cout << e.what() << endl;
	}

	client->shutdown();
	delete client;
}

HttpServer::HttpServer(ClientSocket client) : client(client), stream(client) {
}

void HttpServer::run() {
	bool closing = false;

	while (!closing) {
		ignore_newlines();
		RequestLine request_line(stream.read_to_crlf());

		if (request_line.http2_preface()) {
			String http2_preface_end = stream.peek_string(8);
			if (http2_preface_end == "\r\nSM\r\n\r\n") {
				cout << "Oh my god. An HTTP/2 client!" << endl;
			} else {
				// Partial HTTP/2 preface. Should read as an invalid HTTP/1.1
				// method (WTF is PRI?)
				http_error(400, true);
			}

			closing = true;
		} else if (!request_line.valid()) {
			http_error(400, true);
			closing = true;
		} else {
			vector<String> headers;

			for (String line = stream.read_to_crlf(); line != ""; line = stream.read_to_crlf())
				headers.push_back(line);

			http_error(200, false);
		}
	}
}

void HttpServer::ignore_newlines() {
	while (stream.peek_string(2) == "\r\n")
		stream.skip(2);
}

void HttpServer::parse_request_line() {
	String request_line = stream.read_to_crlf();

	cout << "Request line: " << request_line << endl;
}

void HttpServer::http_error(int code, bool connection_close) {
	String title = http_status_codes[code];
	String description = http_status_descriptions[code];

	String response = String::format("HTTP/1.1 %d %s\r\n", code, (const char *) title);;

	String response_body = String::format("\r\n<!DOCTYPE html><html><head><title>%d %s</title></head><body><h1>%s</h1>\r\n<p>%s</p><hr/><address>CRails/1.0 (Unix) Server at localhost Port 3010</address></body></html>\r\n",
		code, (const char *) title,
		(const char *) title,
		(const char *) description);

	if (connection_close)
		response += "Connection: close\r\n";
  else
    response += String::format("Content-Length: %d\r\n", response_body.length() - 2);

	client.send(response + response_body);
}

