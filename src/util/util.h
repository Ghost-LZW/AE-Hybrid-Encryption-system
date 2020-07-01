#pragma once

#include <string>

#define shift(x) (1<<x)

namespace Util {
	std::string HexToBinary(std::string);
	std::string BinaryToHex(std::string);
}