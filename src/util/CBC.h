#pragma once

#include <string>

namespace CBC {
	std::string encrypt(std::string vec, std::string message,
						std::string& aesKey);
	std::string decrypt(std::string vec, std::string message,
						const std::string& aesKey);
}