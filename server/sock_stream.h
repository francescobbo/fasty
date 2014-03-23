#pragma once

#include "../utils/blob.h"
#include "../utils/string.h"
#include "socket.h"

class SockStream {
public:
	SockStream(Socket &socket);

	String read_to_crlf();
	Blob read_bytes(int count);

private:
	void ensure_chars(int count);

	Socket &socket;
	Blob buffer;
	size_t position;
};
