#pragma once
#include <vector>
#include <string>
#include <bitset>

template<unsigned OUTLEN = 64,
		 unsigned R = 72,
		 unsigned B = 200,
		 uint8_t D = 0x06>
class Keccak {
public:
	using runit = std::bitset<R * 8>;
	using bunit = std::bitset<B / 25 * 8>;
	using content = std::vector<runit>;
	using str = std::string;
	using state = bunit[5][5];
	Keccak();
private:
	unsigned r;
	unsigned c;
	unsigned b;
	unsigned out_len;
	uint8_t d;
	const uint64_t RC[24] = {
        0x0000000000000001, 0x0000000000008082, 0x800000000000808A, 0x8000000080008000,
        0x000000000000808B, 0x0000000080000001, 0x8000000080008081, 0x8000000000008009, 
        0x000000000000008A, 0x0000000000000088, 0x0000000080008009, 0x000000008000000A,
        0x000000008000808B, 0x800000000000008B, 0x8000000000008089, 0x8000000000008003,
        0x8000000000008002, 0x8000000000000080, 0x000000000000800A, 0x800000008000000A,
        0x8000000080008081, 0x8000000000008080, 0x0000000080000001, 0x8000000080008008};
	const uint8_t RT[5][5] = {
        {0, 36, 3, 41, 18}, 
        {1, 44, 10, 45, 2}, 
        {62, 6, 43, 15, 61},
        {28, 55, 25, 21, 56},
        {27, 20, 39, 8, 14}
    };
	void sital(state& value);
	void rou_pai_kai(state& value);
	void yota(state& value, const uint64_t& rc);
	void f(state& A);
	void round(state& value, const uint64_t& rc);

public:
	str encrypt(str value);
};