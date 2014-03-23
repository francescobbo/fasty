#include "http_server.h"

#include <iostream>
using namespace std;

void HttpServer::InitThread(ClientSocket *client) {
	try {
		HttpServer s(*client);
		s.run();
	} catch (std::exception &e) {
		cout << "Connection aborted." << endl;
		cout << e.what() << endl;
		client->shutdown();
	}

	delete client;
}

HttpServer::HttpServer(ClientSocket client) : client(client), stream(client) {
}

void HttpServer::run() {
	while (true) {
		ignore_newlines();
		parse_request_line();
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

