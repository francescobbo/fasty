#include "base64.h"

static char encoding_table[] = {
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
	'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
	'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
	'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
	'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
	'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
	'w', 'x', 'y', 'z', '0', '1', '2', '3',
	'4', '5', '6', '7', '8', '9', '+', '/'
};

static const char decoding_table[256] = {
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,62,-1,-1,-1,63,52,53,54,55,56,57,58,59,60,61,-1,-1,-1,-1,-1,-1,-1,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,
	22,23,24,25,-1,-1,-1,-1,-1,-1,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1
};

static int mod_table[] = {0, 2, 1};

String Base64::encode64(const Blob &bytes) {
	size_t in_len = bytes.size();
	size_t out_len = 4 * ((in_len + 2) / 3);

	String ret;
	ret.ensure_capacity(out_len);

	for (size_t i = 0; i < in_len;) {
		uint32_t octet_a = i < in_len ? (unsigned char)bytes[i++] : 0;
		uint32_t octet_b = i < in_len ? (unsigned char)bytes[i++] : 0;
		uint32_t octet_c = i < in_len ? (unsigned char)bytes[i++] : 0;

		uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;

		ret += encoding_table[(triple >> 3 * 6) & 0x3F];
		ret += encoding_table[(triple >> 2 * 6) & 0x3F];
		ret += encoding_table[(triple >> 1 * 6) & 0x3F];
		ret += encoding_table[(triple >> 0 * 6) & 0x3F];
	}

	for (int i = 0; i < mod_table[in_len % 3]; i++)
		ret[out_len - 1 - i] = '=';

	return ret;
}

Blob Base64::decode64(const String &encoded) {
	size_t in_len = encoded.size();
    size_t out_len = in_len / 4 * 3;

    if (encoded[in_len - 1] == '=') (out_len)--;
    if (encoded[in_len - 2] == '=') (out_len)--;

    Blob ret;
    ret.ensure_capacity(out_len);

    for (size_t i = 0, j = 0; i < in_len;) {
        uint32_t sextet_a = encoded[i] == '=' ? 0 & i++ : decoding_table[(int)encoded[i++]];
        uint32_t sextet_b = encoded[i] == '=' ? 0 & i++ : decoding_table[(int)encoded[i++]];
        uint32_t sextet_c = encoded[i] == '=' ? 0 & i++ : decoding_table[(int)encoded[i++]];
        uint32_t sextet_d = encoded[i] == '=' ? 0 & i++ : decoding_table[(int)encoded[i++]];

        uint32_t triple = (sextet_a << 3 * 6)
	        + (sextet_b << 2 * 6)
	        + (sextet_c << 1 * 6)
	        + (sextet_d << 0 * 6);

        if (j < out_len) ret[j++] = (triple >> 2 * 8) & 0xFF;
        if (j < out_len) ret[j++] = (triple >> 1 * 8) & 0xFF;
        if (j < out_len) ret[j++] = (triple >> 0 * 8) & 0xFF;
    }

    return ret;
}
