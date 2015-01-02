#include "base64.h"
#include <math.h>

#define BASE64_PAD	    '='
#define BASE64DE_FIRST	'+'
#define BASE64DE_LAST	'z'

static char base64en[] = {
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
	'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',	'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
	'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',	'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
	'w', 'x', 'y', 'z', '0', '1', '2', '3',	'4', '5', '6', '7', '8', '9', '+', '/',
};

static signed char base64de[] = {
	    62,  -1,  -1,  -1,  63,  52,  53,  54, 55,  56,  57,  58,  59,  60,  61,  -1,
	    -1,  -1,  -1,  -1,  -1,  -1,   0,  1,  2,   3,   4,   5,   6,   7,   8,   9,
		10,  11,  12,  13,  14,  15,  16,  17, 18,  19,  20,  21,  22,  23,  24,  25,
	   -1,  -1,  -1,  -1,  -1,  -1,  26,   27, 28,  29,  30,  31,  32,  33,  34,  35,
	    36,  37,  38,  39,  40,  41,  42,  43, 44,  45,  46,  47,  48,  49,  50,  51,
};

static const std::string base64_chars(base64en);

static inline bool is_base64(unsigned char c){
	return (isalnum(c) || (c == '+') || (c == '/'));
}

int base64Encode(unsigned char *in, int inlen, char *out){
	int i, j;

	for (i = j = 0; i < inlen; i++){
		int s = i % 3;

		switch (s) {
		case 0:
			out[j++] = base64en[(in[i] >> 2) & 0x3F];
			continue;
		case 1:
			out[j++] = base64en[((in[i-1] & 0x3) << 4) + ((in[i] >> 4) & 0xF)];
			continue;
		case 2:
			out[j++] = base64en[((in[i-1] & 0xF) << 2) + ((in[i] >> 6) & 0x3)];
			out[j++] = base64en[in[i] & 0x3F];
		}
	}

	i -= 1;

	if ((i % 3) == 0) {
		out[j++] = base64en[(in[i] & 0x3) << 4];
		out[j++] = BASE64_PAD;
		out[j++] = BASE64_PAD;
	} else if ((i % 3) == 1) {
		out[j++] = base64en[(in[i] & 0xF) << 2];
		out[j++] = BASE64_PAD;
	}

	out[j] = 0;

	return 0; // OK
}

int base64Decode(char *in, int inlen, unsigned char *out){
	int i, j;

	for (i = j = 0; i < inlen; i++) {
		int c;
		int s = i % 4;

		if (in[i] == '='){
			out[j] = 0;
			return 0; // OK
		}

		if (in[i] < BASE64DE_FIRST || in[i] > BASE64DE_LAST ||
		    (c = base64de[in[i] - BASE64DE_FIRST]) == -1)
			return 1; // INVALID

		switch (s) {
		case 0:
			out[j] = c << 2;
			continue;
		case 1:
			out[j++] += (c >> 4) & 0x3;

			if (i < (inlen - 3) || in[inlen - 2] != '=') out[j] = (c & 0xF) << 4;
			continue;
		case 2:
			out[j++] += (c >> 2) & 0xF;

			if (i < (inlen - 2) || in[inlen - 1] != '=') out[j] =  (c & 0x3) << 6;
			continue;
		case 3:
			out[j++] += c;
		}
	}

	out[j] = 0;

	return 0; // OK
}

std::string base64Encode(unsigned char const* in, unsigned int inlen){
	std::string ret;
	int i = 0;
	int j = 0;
	unsigned char char_array_3[3];
	unsigned char char_array_4[4];

	while (inlen--) {
	char_array_3[i++] = *(in++);
	if (i == 3) {
		char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
		char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
		char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
		char_array_4[3] = char_array_3[2] & 0x3f;

		for(i = 0; (i <4) ; i++)
		ret += base64_chars[char_array_4[i]];
		i = 0;
	}
	}

	if (i)
	{
	for(j = i; j < 3; j++)
		char_array_3[j] = '\0';

	char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
	char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
	char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
	char_array_4[3] = char_array_3[2] & 0x3f;

	for (j = 0; (j < i + 1); j++)
		ret += base64_chars[char_array_4[j]];

	while((i++ < 3))
		ret += '=';

	}

	return ret;
}

std::string base64Decode(std::string const& in){
	int in_len = in.size();
	int i = 0;
	int j = 0;
	int in_ = 0;
	unsigned char char_array_4[4], char_array_3[3];
	std::string ret;

	while (in_len-- && ( in[in_] != '=') && is_base64(in[in_])) {
	char_array_4[i++] = in[in_]; in_++;
	if (i ==4) {
		for (i = 0; i <4; i++)
		char_array_4[i] = base64_chars.find(char_array_4[i]);

		char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
		char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
		char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

		for (i = 0; (i < 3); i++)
		ret += char_array_3[i];
		i = 0;
	}
	}

	if (i) {
	for (j = i; j <4; j++)
		char_array_4[j] = 0;

	for (j = 0; j <4; j++)
		char_array_4[j] = base64_chars.find(char_array_4[j]);

	char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
	char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
	char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

	for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
	}

	return ret;
}

std::string base64Encode(std::string in){
	return base64Encode((unsigned char const *)in.c_str(), in.size());
}