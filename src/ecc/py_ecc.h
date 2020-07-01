#pragma once

#include <string>
#include <algorithm>
#include <utility>
#include <iostream>

namespace ECC {
	std::pair<std::string, std::string> getKey(int N,
										  std::string X, std::string Y,
										  int A = 1, int B = 1);
	std::string encrypt(std::string message,
					   int N, std::string X, std::string Y,
					   int A, int B);
	std::string decrypt(std::string message,
					   int N, std::string X, std::string Y, int A, int B);
}