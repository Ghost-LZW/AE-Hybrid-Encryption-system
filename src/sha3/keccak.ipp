#pragma once
#include <cstdio>
#include <iostream>
#include "keccak.hpp"
#include "../util/util.h"

using unit = std::bitset<sizeof(uint8_t) * 8>;

template<unsigned OUTLEN, unsigned R, unsigned B,
		 uint8_t D>
Keccak<OUTLEN, R, B, D>::Keccak() : 
	r(R), b(B), out_len(OUTLEN), d(D) {
	c = b - r;
	if (c < 0 or b % 5 != 0) throw "error";
}

#define rot(val, step) ((val << step) | (val >> (val.size() - step)))

template<unsigned OUTLEN, unsigned R, unsigned B,
		 uint8_t D>
void Keccak<OUTLEN, R, B, D>::f(Keccak<OUTLEN, R, B, D>::state& A) {
	for (int i = 0; i < 24; i++)
		this->round(A, RC[i]);
}

template<unsigned OUTLEN, unsigned R, unsigned B,
		 uint8_t D>
void Keccak<OUTLEN, R, B, D>::sital(Keccak<OUTLEN, R, B, D>::state& value) {
	std::vector<bunit> c;
	for (int x = 0; x < 5; x++) {
		bunit res;
		for (int y = 0; y < 5; y++)
			res ^= value[y][x];
		c.emplace_back(res);
	}
	std::vector<bunit> dv;
	for (int x = 0; x < 5; x++)
		dv.emplace_back(c[(x + 4) % 5] ^ rot(c[(x + 1) % 5], 1));
	for (int y = 0; y < 5; y++)
		for (int x = 0; x < 5; x++) 
			value[y][x] ^= dv[x];
}

template<unsigned OUTLEN, unsigned R, unsigned B,
		 uint8_t D>
void Keccak<OUTLEN, R, B, D>::rou_pai_kai(Keccak<OUTLEN, R, B, D>::state& value) {
	state Bs;
	for (int x = 0; x < 5; x++)
		for (int y = 0; y < 5; y++)
			Bs[(2 * x + 3 * y) % 5][y] = rot(value[y][x], RT[y][x]);

	for (int x = 0; x < 5; x++)
		for (int y = 0; y < 5; y++)
			value[y][x] = Bs[y][x] ^ ((~Bs[y][(x + 1) % 5]) & Bs[y][(x + 2) % 5]);
}

template<unsigned OUTLEN, unsigned R, unsigned B,
		 uint8_t D>
void Keccak<OUTLEN, R, B, D>::yota(Keccak<OUTLEN, R, B, D>::state& value, const uint64_t& rc) {
	value[0][0] ^= bunit(rc);
}

template<unsigned OUTLEN, unsigned R, unsigned B,
		 uint8_t D>
void Keccak<OUTLEN, R, B, D>::round(Keccak<OUTLEN, R, B, D>::state& value,
									const uint64_t& rc) {
	this->sital(value);
	this->rou_pai_kai(value);
	this->yota(value, rc);
}

template<unsigned OUTLEN, unsigned R, unsigned B,
		 uint8_t D>
std::string Keccak<OUTLEN, R, B, D>::encrypt(std::string value) {
	size_t length = value.length();
	int res = length % 8;
	if (res) std::cerr << "worring content unit is not base on byte or has's been destoryed" << std::endl;
	while (res and res != 8) value += '0', length += 1, res += 1;
	res = r - length / 8 % r;
	if (res == 1) value += unit(0x80 ^ d).to_string();
	else if (res > 1) {
		res -= 2;
		value += unit(d).to_string();
		while (res -- )
			value += unit(0).to_string();
		value += unit(0x80).to_string();
	}

	length = value.length();
	content groups;
	for (unsigned i = 0; i < length; i += r * 8)
		groups.emplace_back(runit(value.substr(i, r * 8)));
	
	state A;
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++)
			A[i][j].reset();

	unsigned w = b / 25;
	for (const auto& g : groups) {
		str part = g.to_string();
		for (int y = 0; y < 5; y++) {
			for (int x = 0; x < 5; x++) if ((y * 5 + x) * w < r) {
				A[y][x] ^= bunit(part.substr((y * 5 + x) * w * 8, w * 8));
			} else break;
		}
		f(A);
	}


	std::string result;
	char buf[33];
	auto ck = [](char t) {return t == 0 ? '0' : t;};
	for (int y = 0; y < 5; y++) {
		for (int x = 0; x < 5; x++) if ((y * 5 + x) * w < out_len) {
			std::string temp = A[y][x].to_string();
			for (unsigned z = 0; z < w; z++) if ((y * 5 + x) * w + z < out_len) {
				std::string part = temp.substr(z * 8, 8);
				int pv = stoi(part, nullptr, 2);
				sprintf(buf, "%X", pv);
				result += ck(buf[0]);
				result += ck(buf[1]);
			} else break;
		} else break;
	}

	return result;
}