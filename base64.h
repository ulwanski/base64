#include <stdio.h>
#include <string.h>
#include <string>

#ifndef __BASE64_H__
#define __BASE64_H__

#define BASE64_ENCODE_OUT_SIZE(s)	(((s) + 2) / 3 * 4)
#define BASE64_DECODE_OUT_SIZE(s)	(((s)) / 4 * 3)

int base64Encode(unsigned char *in, int inlen, char *out);
int base64Decode(char *in, int inlen, unsigned char *out);
std::string base64Encode(unsigned char const* in, unsigned int inlen);
std::string base64Encode(std::string in);
std::string base64Decode(std::string const& in);

#endif /* __BASE64_H__ */