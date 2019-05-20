#include "crypto.h"

Crypto::Crypto()
{

}

Crypto &Crypto::Instance()
{
	static Crypto instance;
	return instance;
}

string Crypto::encode(const string &str)
{
	string result;
	for (unsigned int i = 0; i < str.size(); ++i)
	{
		const char ch = str.at(i);
		result.push_back(ch + CRYPTO_VALUE);
	}
	return str;
}

string Crypto::decode(const string &str)
{
	string result;
	for (unsigned int i = 0; i < str.size(); ++i)
	{
		const char ch = str.at(i);
		result.push_back(ch - CRYPTO_VALUE);
	}
	return str;
}
