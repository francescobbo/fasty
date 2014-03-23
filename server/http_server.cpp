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

HttpServer::HttpServer(ClientSocket client) : client(client) {
}

void HttpServer::run() {
}

