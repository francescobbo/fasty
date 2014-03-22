#include "socket.h"
#include "../utils/errno_exception.h"

#include <sys/unistd.h>
#include <sys/socket.h>

Socket::Socket(int socket) : sock(socket) {
	closed = false;
}

Socket::Socket(const Socket &copy) {
	closed = copy.closed;
	sock = copy.sock;
}

Socket::Socket(Socket &&move) {
	closed = move.closed;
	sock = move.sock;

	move.closed = true;
	move.sock = -1;
}

void Socket::send(const Blob &data) {
	if (::send(sock, data.ptr(), data.size(), MSG_NOSIGNAL) < 0)
		throw ErrnoException("Error while sending data to client", errno);
}

void Socket::send(const String &data) {
	if (::send(sock, data, data.length(), MSG_NOSIGNAL) < 0)
		throw ErrnoException("Error while sending data to client", errno);
}

Blob Socket::receive() {
	char buffer[1024];
	
	int read = recv(sock, buffer, 1024, 0);
	if (read < 0)
		throw ErrnoException("Error while receiving data from client", errno);

	buffer[read] = '\0';

	return Blob(buffer);
}

void Socket::shutdown() {
	if (!closed and sock >= 0) {
		closed = true;
		close(sock);
	}
}

