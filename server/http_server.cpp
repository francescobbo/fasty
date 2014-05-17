#include "http_server.h"
#include "http2_server.h"
#include "http_headers.h"
#include "request_line.h"
#include "exceptions.h"
#include "../utils/http_status_codes.h"

#include <iostream>
using namespace std;

extern int http_server_port;
extern int https_server_port;

void HttpServer::InitThread(ClientSocket client) {
	cout << "[S] Connection started" << endl;

	try {
		HttpServer s(client);
		s.run();
	} catch (const ConnectionClosedException &e) {
		cout << "[S] Connection closed. Bye :)" << endl;
	} catch (std::exception &e) {
		cout << "[S] Connection aborted." << endl;
		cout << e.what() << endl;
	} catch (const char *s) {
		cout << "[S] Connection aborted." << endl;
		cout << s << endl;
	} catch (String &s) {
		cout << "[S] Connection aborted." << endl;
		cout << s << endl;
	} catch (...) {
		cout << "[S] Connection aborted due to unknown exception" << endl;
	}

	client.shutdown();
	_Exit(0);
}

void HttpServer::InitRedirectThread(ClientSocket client) {
	cout << "[R] Connection started" << endl;

	try {
		HttpServer s(client);
		s.run_redirect();
	} catch (const ConnectionClosedException &e) {
		cout << "[R] Connection closed. Bye :)" << endl;
	} catch (std::exception &e) {
		cout << "[R] Connection aborted." << endl;
		cout << e.what() << endl;
	} catch (String &s) {
		cout << "[R] Connection aborted." << endl;
		cout << s << endl;
	} catch (...) {
		cout << "[R] Connection aborted due to unknown exception" << endl;
	}

	client.shutdown();
	_Exit(0);
}

HttpServer::HttpServer(ClientSocket &client) : client(client), stream(client) {
}

void HttpServer::run() {
	bool closing = false;

	while (!closing) {
		ignore_newlines();
		RequestLine request_line(stream.read_to_crlf());

		if (request_line.http2_preface()) {
			String http2_preface_end = stream.peek_string(8);
			if (http2_preface_end == "\r\nSM\r\n\r\n") {
				stream.skip(8);
				Http2Server http2(stream, client);
				http2.run();
				closing = true;
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
			HttpHeaders headers;

			for (String line = stream.read_to_crlf(); line != ""; line = stream.read_to_crlf())
				headers.add(line);

			if (!headers.has_header("host")) {
				http_error(400, true);
				closing = true;
			}

			http_error(200, false);
		}
	}
}

void HttpServer::run_redirect() {
	ignore_newlines();
	RequestLine request_line(stream.read_to_crlf());

	if (!request_line.valid()) {
		http_error(400, true);
	} else {
		HttpHeaders headers;

		for (String line = stream.read_to_crlf(); line != ""; line = stream.read_to_crlf())
			headers.add(line);

		if (!headers.has_header("host"))
			http_error(400, true);
		else
			https_redirect(headers["host"], request_line.path());
	}
}

void HttpServer::ignore_newlines() {
	while (stream.peek_string(2) == "\r\n")
		stream.skip(2);
}

void HttpServer::http_error(int code, bool connection_close) {
	String title = http_status_codes[code];
	String description = http_status_descriptions[code];

	String response = String::format("HTTP/1.1 %d %s\r\n", code, (const char *) title);;

	String response_body = String::format("\r\n<!DOCTYPE html><html><head><title>%d %s</title></head><body><h1>%s</h1>\r\n<p>%s</p><hr/><address>CRails/1.0 (Unix) Server at localhost Port %d</address></body></html>\r\n",
		code, (const char *) title,
		(const char *) title,
		(const char *) description, https_server_port);

	if (connection_close)
		response += "Connection: close\r\n";
  else
    response += String::format("Content-Length: %d\r\n", response_body.length() - 2);

	client.send(response + response_body);
}

void HttpServer::https_redirect(const String &host, const String &path) {
	String new_host = host;

	int colon = new_host.index(':');
	if (colon > 0)
		new_host = new_host.substr(0, colon);

	if (https_server_port != 443)
		new_host += String(":") + https_server_port;

	String response = String::format("HTTP/1.1 301 Moved Permanently\r\nConnection: close\r\nContent-Length: 0\r\nLocation: https://%s%s\r\nServer: fasty\r\n\r\n", (const char *) new_host, (const char *) path);
	client.send(response);
}