#include "client_socket.h"
#include "../utils/errno_exception.h"

#include <openssl/ssl.h>
#include <sys/socket.h>

extern SSL_CTX *ssl_ctx;

ClientSocket::ClientSocket(int socket, String ip, bool use_ssl) : Socket(socket), ip(ip) {
	if (use_ssl) {
		ssl = SSL_new(ssl_ctx);
		SSL_set_fd(ssl, socket);
		SSL_accept(ssl);
	}
}

ClientSocket::ClientSocket(const ClientSocket &copy) : Socket(copy), ssl(copy.ssl), ip(copy.ip) {
}

ClientSocket::ClientSocket(ClientSocket &&move) : Socket(move), ssl(move.ssl), ip(move.ip) {
	move.ssl = nullptr;
}

ClientSocket::~ClientSocket() {}

String ClientSocket::get_ip() {
	return ip;
}

void ClientSocket::send(const char *data, int length) {
	if (ssl) {
		if (SSL_write(ssl, data, length) <= 0)
			throw "SSL Error while sending data to client.";
	} else {
		if (::send(sock, data, length, MSG_NOSIGNAL) < 0)
			throw ErrnoException("Error while sending data to client", errno);
	}
}

void ClientSocket::send(const Blob &data) {
	send((const char *) data.ptr(), data.size());
}

void ClientSocket::send(const String &data) {
	send(data, data.length());
}

Blob ClientSocket::receive() {
	char buffer[1024];

	int read;
	if (ssl) {
		read = SSL_read(ssl, buffer, 1024);
		if (read <= 0)
			throw "SSL Error while receiving data from client";
	} else {
		read = recv(sock, buffer, 1024, 0);
		if (read < 0)
			throw ErrnoException("Error while receiving data from client", errno);
	}

	return Blob(buffer, read);
}

void ClientSocket::shutdown() {
	if (!closed and ssl) {
		SSL_shutdown(ssl);
		SSL_free(ssl);
	}

	Socket::shutdown();
}