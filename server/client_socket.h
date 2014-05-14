#pragma once

#include "../utils/string.h"
#include "socket.h"
#include <openssl/ssl.h>

class ClientSocket : public Socket {
public:
	ClientSocket(int socket, String ip, bool use_ssl = true);
	ClientSocket(const ClientSocket &copy);
	ClientSocket(ClientSocket &&move);
	virtual ~ClientSocket();

	void send(const char *data, int length);
	void send(const String &s);
	void send(const Blob &s);

	Blob receive();

	virtual void shutdown();

	String get_ip();

	SSL *ssl = nullptr;
private:
	String ip;
};
