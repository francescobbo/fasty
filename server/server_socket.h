#pragma once

#include "client_socket.h"
#include "socket.h"

class ServerSocket : public Socket {
public:
	ServerSocket(int port);

	ClientSocket next();

private:
	void boot();

	int port;
	int max_clients;
};
