#pragma once

#include "sock_stream.h"
#include "client_socket.h"
#include "http2_frame.h"

class Http2Server {
public:
	Http2Server(SockStream &in, ClientSocket &out);

	void run();

private:
	SockStream &in;
	ClientSocket &out;

	Http2Frame read_frame();
	void read_first_settings();
	void send_preface();
	void send_goaway(Http2Errors error);
};
