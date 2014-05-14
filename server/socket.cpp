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

Socket::~Socket() {}

void Socket::shutdown() {
	if (!closed and sock >= 0) {
		closed = true;
		close(sock);
	}
}

