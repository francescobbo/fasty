#include "request.h"

Request request;

Request::Request() {}

/**
 * Much of this file was either inspired or directly copied from Rails or Rack
 */

const HttpHeaders &Request::headers() const {
	return _headers;
}

void Request::add_header(const String &line) {
	_headers.add(line);
}

const String &Request::host() const {
	return _headers["host"];
}

void Request::reset() {
	_headers.clear();
}