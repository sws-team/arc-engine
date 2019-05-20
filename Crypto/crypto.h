#ifndef CRYPTO_H
#define CRYPTO_H

#include "stdheader.h"

class Crypto
{
public:
	static Crypto &Instance();

	static string encode(const string& str);
	static string decode(const string& str);

	constexpr static int CRYPTO_VALUE = 12;

private:
	Crypto();
};

#endif // CRYPTO_H
