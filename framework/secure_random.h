#pragma once

#include "../utils/blob.h"

class SecureRandom {
public:
	static void seed();

	static String base64(int length = 16);
	static String hex(int length = 16);
	static Blob random_bytes(int length = 16);
	static String uuid();
};
