#pragma once

#include <chrono>
#include <random>
#include <string>
#include "../sha3/keccak.ipp"

template<unsigned SIZE=20>
class Random {
public:
	Random(uint32_t seed = 
		  (uint32_t)std::chrono::steady_clock::now().time_since_epoch().count());
	std::string next();
private:
	Keccak<SIZE> keccak;
	uint32_t count;
};