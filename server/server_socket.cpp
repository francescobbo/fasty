#include "server_socket.h"
#include "../utils/errno_exception.h"

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>

ServerSocket::ServerSocket(int port, bool use_ssl) : Socket(0), ssl(use_ssl), port(port) {
	if (port <= 0 || port > 65535)
		throw ErrnoException("Port number out of range", ENOTSUP);

	boot();
}

ClientSocket ServerSocket::next() {
	sockaddr_in remote;
	int t = sizeof(remote);

	int client = accept(sock, (sockaddr *) &remote, (socklen_t *) &t);
	if (client < 0)
		throw ErrnoException("Unable to accept a new client", errno);

	char ip[INET_ADDRSTRLEN];
	memset(ip, 0, INET_ADDRSTRLEN);
	inet_ntop(AF_INET, &remote.sin_addr.s_addr, ip, INET_ADDRSTRLEN);
	
	return ClientSocket(client, ip, ssl);
}

void ServerSocket::boot() {
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0)
		throw ErrnoException("Unable to create the socket", errno);

	int value = 1;
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const void *) &value, sizeof(value)) < 0)
		throw ErrnoException("Unable to set the SO_REUSEADDR flag", errno);

	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(port);

	if (bind(sock, (sockaddr *) &server, sizeof(server)) < 0)
		throw ErrnoException("Unable to bind the socket to the port", errno);

	if (listen(sock, SOMAXCONN) < 0)
		throw ErrnoException("Unable to listen from the port", errno);
}

