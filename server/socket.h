#pragma once

#include "../utils/blob.h"
#include "../utils/string.h"

class Socket {
public:
	Socket(int socket);
	Socket(const Socket &copy);
	Socket(Socket &&move);
	virtual ~Socket();

	virtual void shutdown();

protected:
	int sock;
	bool closed;
};

