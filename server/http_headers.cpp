#include "http_headers.h"

void HttpHeaders::add(String header_line) {
	header_line.trim_s();

	int colon = header_line.index(':');
	if (colon < 0)	/* Ignore invalid header */
		return;
	else if (header_line.index(':') == 0) {
		colon = header_line.index(':', 1);
		if (colon < 0)	/* Ignore invalid header */
			return;
	}

	String header_name = header_line.substr(0, colon).trim_s().lowercase_s();
	String header_value = header_line.substr(colon + 1).trim_s();

	if (header_name.is_empty() or header_value.is_empty() or header_name.contains(" "))
		return;

	raw_headers[header_name] = header_value;
}

bool HttpHeaders::has_header(const String &header) {
	return raw_headers.count(header.lowercase());
}

const String &HttpHeaders::operator[](const String &header) const {
	auto it = raw_headers.find(header);
	if (it == raw_headers.end())
		return String::nil;
	else
		return it->second;
}
