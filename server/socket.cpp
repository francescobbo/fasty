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

		/**
		 * The socket is shared among processes. close only releases the socket
		 * in the current process. Shutdown effectively closes communicaion 
		 * with the peer on all processes. However the socket number is not
		 * releases. Need to fix this...
		 * Big thanks to:
		 * http://stackoverflow.com/questions/4160347/close-vs-shutdown-socket#comment4491371_4160356
		 */
		::shutdown(sock, 2);
		close(sock);
	}
}

