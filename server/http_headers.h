#pragma once

#include <map>

#include "../utils/string.h"

class HttpHeaders {
public:
	void add(const String header_line);
	bool has_header(const String &header);
	String &operator[](const String &header);

private:
	std::map<String, String> raw_headers;
};