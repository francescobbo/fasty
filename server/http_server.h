#pragma once

#include "client_socket.h"
#include "sock_stream.h"

class HttpServer {
public:
	static void InitThread(ClientSocket *client);

	HttpServer(ClientSocket socket);

private:
	ClientSocket client;
	SockStream stream;

	void run();

	void ignore_newlines();
	void parse_request_line();
};

