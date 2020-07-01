#include "../random/rand.ipp"
#include "../aes/aes.h"
#include "../util/util.h"
#include <iostream>
#include <cassert>

int main(int argc, char const *argv[]) {
	Random<16> rand;
	AES aes;
	std::string val = Util::HexToBinary(rand.next());
	std::string after = aes.encrypt(val);
	assert(val != aes.decrypt(after));

	return 0;
}