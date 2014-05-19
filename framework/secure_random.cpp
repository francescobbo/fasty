#include "base64.h"
#include "secure_random.h"

#include <openssl/rand.h>

void SecureRandom::seed() {
	RAND_poll();
}

String SecureRandom::base64(int length) {
	return Base64::encode64(random_bytes(length));
}

String SecureRandom::hex(int length) {
	Blob bytes = random_bytes(length);
	String ret;
	ret.ensure_capacity(length * 2 + 1);

	for (int i = 0; i < length; i++)
		ret += String::format("%02x", (unsigned int)bytes[i]);

	return ret;
}

Blob SecureRandom::random_bytes(int length) {
	Blob ret;
	ret.ensure_capacity(length);
	ret.size(length);

	RAND_bytes(ret.ptr(), length);

	return ret;
}

String SecureRandom::uuid() {
	Blob bytes = random_bytes(16);
	uint32_t parts[6];

	parts[0] = *(uint32_t *) &bytes[0];
	parts[1] = *(uint16_t *) &bytes[4];
	parts[2] = *(uint16_t *) &bytes[6];
	parts[3] = *(uint16_t *) &bytes[8];
	parts[4] = *(uint16_t *) &bytes[10];
	parts[5] = *(uint32_t *) &bytes[12];

	parts[2] = (parts[2] & 0x0fff) | 0x4000;
	parts[3] = (parts[3] & 0x3fff) | 0x8000;

	return String::format("%08x-%04x-%04x-%04x-%04x%08x", parts[0], parts[1], parts[2], parts[3], parts[4], parts[5]);
}
