#pragma once

#include "../utils/string.h"
#include <vector>

class RequestLine {
public:
	RequestLine(const String &line);

	bool valid() const;
	bool http2_preface() const;

	String method() const;
	String path() const;
	String full_path() const;
	String host() const;
	String protocol() const;

	int http_version() const;
	bool has_known_method() const;

private:
	std::vector<String> elements;
};
