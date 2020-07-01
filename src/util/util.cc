#include "util.h"

#include <iostream>
#include <cstdio>
#include <bitset>

auto ck = [](char t) {return t == 0 ? '0' : t;};
auto turn = [](char t) {return isdigit(t) ? t - '0' :
							   isupper(t) ? t - 'A' + 10 :
							   t - 'a' + 10;};

std::string Util::HexToBinary(std::string hex) {
	std::string result;
	for (const auto i : hex) {
		result += std::bitset<4>(turn(i)).to_string();
	}
	return result;
}

std::string Util::BinaryToHex(std::string Binary) {
	int len = Binary.length();
	std::string result;
	char buf[33];
	for (int i = 0; i < len; i += 4) {
		std::string part = Binary.substr(i, 4);
		int pv = stoi(part, nullptr, 2);
		sprintf(buf, "%X", pv);
		result += ck(buf[0]);
	}
	return result;
}