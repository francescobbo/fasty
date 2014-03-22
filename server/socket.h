#pragma once

#include "../utils/blob.h"
#include "../utils/string.h"

class Socket {
public:
	Socket(int socket);
	Socket(const Socket &copy);
	Socket(Socket &&move);

	void send(const String &s);
	void send(const Blob &s);
	Blob receive();

	void shutdown();

protected:
	int sock;

private:
	bool closed;
};

