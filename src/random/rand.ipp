#pragma once
#include "rand.hpp"

template<unsigned SIZE>
Random<SIZE>::Random(uint32_t seed) :
	count(seed) {
}

template<unsigned SIZE>
std::string Random<SIZE>::next() {
	count += 1;
	return keccak.encrypt(std::bitset<32 * 8>(count).to_string());
}