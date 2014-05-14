#pragma once

#include "../utils/string.h"
#include "socket.h"

class ClientSocket : public Socket {
public:
	ClientSocket(int socket, String ip);
	ClientSocket(const ClientSocket &copy);
	ClientSocket(ClientSocket &&move);
	virtual ~ClientSocket();

	void send(const char *data, int length);
	void send(const String &s);
	void send(const Blob &s);

	Blob receive();

	String get_ip();

private:
	String ip;
};
