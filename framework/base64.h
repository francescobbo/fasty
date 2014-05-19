#pragma once

#include "../utils/blob.h"
#include "../utils/string.h"

class Base64 {
public:
	static Blob decode64(const String &encoded);
	static String encode64(const Blob &bytes);
};
