#include "sock_stream.h"
#include <sys/socket.h>

SockStream::SockStream(Socket &socket) : socket(socket), buffer("") {
	position = 0;
}

String SockStream::read_to_crlf() {
	String result = "";

	while (true) {
		ensure_chars(2);

		int line_end = buffer.index("\r\n", position);
		if (line_end >= 0) {
			result += (const char *) buffer.substr(position, line_end - position).ptr();
			position = line_end + 2;
			return result;
		}

		result += (const char *) buffer.substr(position).ptr();
		position = buffer.size();
	}
}

Blob SockStream::read_bytes(int count) {
	Blob result;
	result.ensure_capacity(count);
	ensure_chars(count);

	result = buffer.substr(position, count);
	position += count;

	return result;
}

void SockStream::ensure_chars(int count) {
	if (position + count > buffer.size())
		buffer += socket.receive();
}
