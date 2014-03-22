#include "client_socket.h"

ClientSocket::ClientSocket(int socket, String ip) : Socket(socket), ip(ip) {
}

ClientSocket::ClientSocket(const ClientSocket &copy) : Socket(copy), ip(copy.ip) {
}

ClientSocket::ClientSocket(ClientSocket &&move) : Socket(move), ip(move.ip) {
}

String ClientSocket::get_ip() {
	return ip;
}
