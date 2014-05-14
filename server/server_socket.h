#pragma once

#include "client_socket.h"
#include "socket.h"

class ServerSocket : public Socket {
public:
	ServerSocket(int port, bool use_ssl);

	ClientSocket next();

private:
	void boot();

	bool ssl;
	int port;
	int max_clients;
};
