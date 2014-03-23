#pragma once

#include "string.h"

class Uri {
public:
	Uri(const String &uri);

	String scheme() const;
	String host() const;
	String host_with_port(bool skip_default = false) const;
	int port() const;
	String path() const;
	String query() const;
	String fragment() const;
	String request_uri() const;

private:
	bool is_default_port() const;

	String _scheme, _host, _path, _query, _fragment;
	int _port;
};