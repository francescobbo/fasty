#include "http2_server.h"
#include "http2_frame.h"

#include <iostream>
using namespace std;

Http2Server::Http2Server(SockStream &in, ClientSocket &out) : in(in), out(out) {
}

void Http2Server::run() {
	cout << "Greeat" << endl;
	read_first_settings();
	send_preface();

	while (true) {
		Http2Frame f = read_frame();
		cout << "Received frame. Type: " << (int)f.header.type << endl;
		if (f.header.type == FrameTypes::GoAway) {
			cout << "Received GoAway. Error: " << *((unsigned int *)&f.data[4]) << endl;
			break;
		}
	}
}

void Http2Server::read_first_settings() {
	Http2Frame f = read_frame();

	if (f.header.type != FrameTypes::Settings) {
		throw "This is not HTTP2!";
	} else {
		cout << "Got settings! Valid HTTP2" << endl;
	}
}

void Http2Server::send_preface() {
	out.send("PRI * HTTP/2.0\r\n\r\nSM\r\n\r\n");
	Http2FrameHeader settings(FrameTypes::Settings, 0);
	out.send((const char *) &settings, sizeof(settings));
}

Http2Frame Http2Server::read_frame() {
	Http2Frame frame;

	in.read_bytes((unsigned char *) &frame.header, sizeof(frame.header));
	if (frame.header.get_length() != 0) {
		frame.data = new unsigned char[frame.header.get_length()];
		in.read_bytes(frame.data, frame.header.get_length());
	}

	return frame;
}

void Http2Server::send_goaway(Http2Errors error) {
	Http2GoAwayFrame goaway(error);
	out.send((const char *) &goaway, sizeof(goaway));
}

