#include "client_socket.h"
#include "../utils/errno_exception.h"

ClientSocket::ClientSocket(int socket, String ip) : Socket(socket), ip(ip) {
}

ClientSocket::ClientSocket(const ClientSocket &copy) : Socket(copy), ip(copy.ip) {
}

ClientSocket::ClientSocket(ClientSocket &&move) : Socket(move), ip(move.ip) {
}

ClientSocket::~ClientSocket() {}

String ClientSocket::get_ip() {
	return ip;
}

void ClientSocket::send(const char *data, int length) {
	if (::send(sock, data, length, MSG_NOSIGNAL) < 0)
		throw ErrnoException("Error while sending data to client", errno);
}

void ClientSocket::send(const Blob &data) {
	send((const char *) data.ptr(), data.size());
}

void ClientSocket::send(const String &data) {
	send(data, data.length());
}

Blob ClientSocket::receive() {
	char buffer[1024];

	int read = recv(sock, buffer, 1024, 0);
	if (read < 0)
		throw ErrnoException("Error while receiving data from client", errno);

	return Blob(buffer, read);
}
