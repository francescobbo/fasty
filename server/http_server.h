#pragma once

#include "client_socket.h"

class HttpServer {
public:
	static void InitThread(ClientSocket *client);

	HttpServer(ClientSocket socket);

private:
	ClientSocket client;

	void run();
};

