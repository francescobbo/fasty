#pragma once

#include <map>

#include "../utils/string.h"

class HttpHeaders {
public:
	void add(const String header_line);
	bool has_header(const String &header);
	const String &operator[](const String &header) const;

	inline void clear() {
		raw_headers.clear();
	}

private:
	std::map<String, String> raw_headers;
};