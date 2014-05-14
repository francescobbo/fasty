#pragma once

#include <string.h>
#include <arpa/inet.h>

#include <iostream>
using namespace std;

enum class FrameTypes : unsigned char {
	Data = 0,
	Headers = 1,
	Priority = 2,
	RstStream = 3,
	Settings = 4,
	PushPromise = 5,
	Ping = 6,
	GoAway = 7,
	WindowUpdate = 8,
	Continuation = 9,
	AltSvc = 10,
	Blocked = 11
};

enum class Http2Errors {
	NoError = 0,
	ProtocolError,
	InternalError,
	FlowControlError,
	SettingsTimeout,
	StreamClosed,
	FrameSizeError,
	RefusedStream,
	Cancel,
	CompressionError,
	ConnectError,
	EnhanceYourCalm,
	InadequateSecurity
};

class Http2FrameHeader {
public:
	Http2FrameHeader() {
	}

	Http2FrameHeader(FrameTypes type, int length) : type(type) {
		set_length(length);
		cout << "Setting length to: " << length << endl;
	}

	unsigned short get_length() const {
		return ntohs(length) & 0x3FFF;
	}

	void set_length(unsigned short val) {
		length = htons(val) & 0x3FFF;
	}

	unsigned int get_stream_id() const {
		return ntohl(stream_id) & 0x7FFFFFFF;
	}

	void set_stream_id(unsigned int val) {
		stream_id = htonl(val) & 0x7FFFFFFF;
	}

	unsigned short length = 0;
	FrameTypes type;
	unsigned char flags = 0;
	unsigned int stream_id = 0;
} __attribute__ ((__packed__));

class Http2Frame {
public:
	Http2Frame() : header() {}

	Http2Frame(Http2Frame &&move) {
		memcpy(&this->header, &move.header, sizeof(move.header));
		this->data = move.data;
		move.data = nullptr;
	}

	~Http2Frame() {
		delete[] data;
	}

	Http2FrameHeader header;
	unsigned char *data = nullptr;
};

class Http2GoAwayFrame : public Http2FrameHeader {
public:
	Http2GoAwayFrame(Http2Errors error) : Http2FrameHeader(FrameTypes::GoAway, 8) {
		set_error_code(error);
	}

	Http2Errors get_error_code() const {
		return (Http2Errors) ntohl(error_code);
	}

	void set_error_code(Http2Errors val) {
		error_code = htonl((unsigned int) val);
	}

	int last_stream_id = 0;
	int error_code = 0;
} __attribute__ ((__packed__));

