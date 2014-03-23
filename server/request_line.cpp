#include "request_line.h"
#include "../utils/uri.h"

RequestLine::RequestLine(const String &line) {
	String copy = line;
	copy.trim();
	elements = copy.split(' ', true);
}

bool RequestLine::valid() const {
	if (elements.size() != 3)
		return false;

	if (elements[2].size() != 8 or elements[2].substr(0, 5) != "HTTP/" or elements[2][6] != '.')
		return false;

	return true;
}

bool RequestLine::http2_preface() const {
	return elements[0] == "PRI" and elements[1] == "*" and elements[2] == "HTTP/2.0";
}

String RequestLine::method() const {
	return elements[0];
}

String RequestLine::full_path() const {
	return elements[1];
}

String RequestLine::path() const {
	return Uri(elements[1]).request_uri();
}

String RequestLine::host() const {
	String host = Uri(elements[1]).host_with_port(true);
	host.lowercase();
	return host;
}

String RequestLine::protocol() const {
	return elements[2];
}

int RequestLine::http_version() const {
	int major = elements[2][5] - '0';
	int minor = elements[2][7] - '0';

	return (major << 4) | minor;
}

bool RequestLine::has_known_method() const {
	const char *valid_methods[] = {
		"GET", "POST", "PUT", "PATCH", "DELETE", "HEAD"
	};

	for (int i = 0; i < 6; i++) {
		if (elements[0] == valid_methods[i])
			return true;
	}

	return false;
}

