#pragma once

#include "../server/http_headers.h"

class HttpServer;

class Request {
friend class HttpServer;

public:
	Request();

	const String &authorization() const;
	const HttpHeaders &headers() const;

	const String &host() const;

	void reset();

private:
	HttpHeaders _headers;

	void add_header(const String &line);
};

extern Request request;