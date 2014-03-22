#pragma once

#include "../utils/string.h"
#include "socket.h"

class ClientSocket : public Socket {
public:
	ClientSocket(int socket, String ip);
	ClientSocket(const ClientSocket &copy);
	ClientSocket(ClientSocket &&move);

	String get_ip();

private:
	String ip;
};
