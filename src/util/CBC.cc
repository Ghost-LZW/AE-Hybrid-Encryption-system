#include "CBC.h"
#include "../aes/aes.h"
#include <bitset>
#include <iostream>

using unit = std::bitset<128>;
using std::string;
using std::cerr;
using std::endl;

std::string CBC::encrypt(std::string vec, std::string message,
						 std::string& aesKey) {
	AES aes; aesKey = aes.getKey();
	std::string result;
	message += "10000000";
	while (message.length() % 128 != 0)
		message += "00000000";
	string pre;
	for (unsigned i = 0; i < message.length(); i += 128) {
		pre = aes.encrypt((unit(message.substr(i, 128)) ^
						  unit(vec)).to_string());
		result += pre;
		vec = pre;			
	}
	return result;
}

std::string CBC::decrypt(std::string vec, std::string message,
						const std::string& aesKey) {
	AES aes(aesKey);
	std::string result;
	string pre, res;

	for (int i = 0; i < message.length(); i += 128) {
		pre = message.substr(i, 128);
		try {
			res = aes.decrypt(pre);
		} catch (char* s) {
			std::cerr << s << std::endl;
		}
		result += (unit(res) ^ unit(vec)).to_string();
		vec = pre;
	}
	unsigned pos = 0;
	while (result[result.length() - pos - 1] == '0') {
		++pos;
	}
	result = result.substr(0, result.length() - pos - 1);
	return result;
}