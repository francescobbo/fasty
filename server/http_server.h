#pragma once

#include "client_socket.h"
#include "sock_stream.h"

class HttpServer {
public:
	static void InitThread(ClientSocket *client);
	static void InitRedirectThread(ClientSocket *client);

	HttpServer(ClientSocket socket);

private:
	ClientSocket client;
	SockStream stream;

	void run();
	void run_redirect();
	void http_error(int code, bool connection_close);
	void https_redirect(const String &host, const String &path);

	void ignore_newlines();
};

