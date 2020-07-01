#include "aes.h"
#include "../random/rand.ipp"
#include "../util/util.h"

using byte = std::bitset<8>;
using word = std::bitset<32>;
using state = byte[4 * 4];
using str = std::string;

AES::AES(std::string KEY) : KEY(KEY) {
	if (KEY.length() == 0) KEY = Random<16>().next(), this->KEY = KEY;
	if (KEY.length() != 32) throw "key length error";
	for (int i = 0; i < 4; i++)
		key[i] = word(stoul(KEY.substr(i * 8, 8), nullptr, 16));
	keyExpand();
}

word AES::Word (byte& k1, byte& k2, byte& k3, byte& k4) {
	word result;
	word temp;
	temp = k1.to_ulong();
	temp <<= 24;
	result |= temp;
	temp = k2.to_ulong();
	temp <<= 16;
	result |= temp;
	temp = k3.to_ulong();
	temp <<= 8;
	result |= temp;
	temp = k4.to_ulong();
	result |= temp;
	return result;
}

word AES::RotWord(const word& rw) {
	word high = rw << 8;
	word low = rw >> 24;
	return high | low;
}

word AES::SubWord(const word& sw) {
	word temp;
	for(int i = 0; i < 32; i += 8) {
		int row = sw[i + 7] * 8 + sw[i + 6] * 4 + sw[i + 5] * 2 + sw[i + 4];
		int col = sw[i + 3] * 8 + sw[i + 2] * 4 + sw[i + 1] * 2 + sw[i];
		byte val = S_Box[row][col];
		for(int j=0; j<8; ++j)
			temp[i + j] = val[j];
	}
	return temp;
}

void AES::keyExpand() {
	for (unsigned i = 0; i < key.size(); i++)
		w[i] = key[i];
	for (unsigned i = NK; i < 4 * (NR + 1); i++) {
		if(i % NK == 0)
			w[i] = w[i-NK] ^ SubWord(RotWord(w[i - 1])) ^ Rcon[i / NK - 1];
		else if (NK> 6 and i % NK == 4)
			w[i] = w[i-NK] ^ SubWord(w[i - 1]);
		else 
			w[i] = w[i-NK] ^ w[i - 1];
	}
}

void AES::SubBytes(state mtx) {
	for(int i = 0; i<16; ++i) {
		int row = mtx[i][7] * 8 + mtx[i][6]*4 + mtx[i][5]*2 + mtx[i][4];
		int col = mtx[i][3] * 8 + mtx[i][2]*4 + mtx[i][1]*2 + mtx[i][0];
		mtx[i] = S_Box[row][col];
	}
}

void AES::InvSubBytes(state mtx) {
	for(int i=0; i<16; ++i) {
		int row = mtx[i][7] * 8 + mtx[i][6] * 4 + mtx[i][5] * 2 + mtx[i][4];
		int col = mtx[i][3] * 8 + mtx[i][2] * 4 + mtx[i][1] * 2 + mtx[i][0];
		mtx[i] = Inv_S_Box[row][col];
	}
}

void AES::ShiftRows(state mtx) {
	byte temp = mtx[4];
	for(int i = 0; i < 3; ++i)
		mtx[i + 4] = mtx[i + 5];
	mtx[7] = temp;

	for(int i = 0; i < 2; ++i) {
		temp = mtx[i + 8];
		mtx[i + 8] = mtx[i + 10];
		mtx[i + 10] = temp;
	}

	temp = mtx[15];
	for(int i = 3; i > 0; --i)
		mtx[i + 12] = mtx[i + 11];
	mtx[12] = temp;
}

void AES::InvShiftRows(state mtx) {
	byte temp = mtx[7];
	for(int i = 3; i > 0; --i)
		mtx[i + 4] = mtx[i + 3];
	mtx[4] = temp;
	for(int i = 0; i < 2; ++i) {
		temp = mtx[i + 8];
		mtx[i + 8] = mtx[i + 10];
		mtx[i + 10] = temp;
	}
	temp = mtx[12];
	for(int i = 0; i < 3; ++i)
		mtx[i + 12] = mtx[i + 13];
	mtx[15] = temp;
}

void AES::AddRoundKey(state mtx, int round) {
	int base = round * 4;
	for(int i = 0; i < 4; ++i) {
		byte k1 = byte((w[base + i] >> 24).to_ulong());
		byte k2 = byte(((w[base + i] << 8) >> 24).to_ulong());
		byte k3 = byte(((w[base + i] << 16) >> 24).to_ulong());
		byte k4 = byte(((w[base + i] << 24) >> 24).to_ulong());
		
		mtx[i] = mtx[i] ^ k1;
		mtx[i + 4] = mtx[i + 4] ^ k2;
		mtx[i + 8] = mtx[i + 8] ^ k3;
		mtx[i + 12] = mtx[i + 12] ^ k4;
	}
}

byte AES::GFMul(byte a, byte b) { 
	byte p = 0;
	byte hi_bit_set;
	for (int counter = 0; counter < 8; counter++) {
		if ((b & byte(1)) != 0) {
			p ^= a;
		}
		hi_bit_set = (byte) (a & byte(0x80));
		a <<= 1;
		if (hi_bit_set != 0) {
			a ^= 0x1b;
		}
		b >>= 1;
	}
	return p;
}

void AES::MixColumns(state mtx) {
	byte arr[4];
	for(int i = 0; i < 4; ++i) {
		for(int j = 0; j < 4; ++j)
			arr[j] = mtx[i+j*4];

		mtx[i] = GFMul(0x02, arr[0]) ^ GFMul(0x03, arr[1]) ^ arr[2] ^ arr[3];
		mtx[i+4] = arr[0] ^ GFMul(0x02, arr[1]) ^ GFMul(0x03, arr[2]) ^ arr[3];
		mtx[i+8] = arr[0] ^ arr[1] ^ GFMul(0x02, arr[2]) ^ GFMul(0x03, arr[3]);
		mtx[i+12] = GFMul(0x03, arr[0]) ^ arr[1] ^ arr[2] ^ GFMul(0x02, arr[3]);
	}
}

void AES::InvMixColumns(state mtx) {
	byte arr[4];
	for(int i = 0; i < 4; ++i) {
		for(int j = 0; j < 4; ++j)
			arr[j] = mtx[i+j*4];

		mtx[i] = GFMul(0x0e, arr[0]) ^ GFMul(0x0b, arr[1]) 
			^ GFMul(0x0d, arr[2]) ^ GFMul(0x09, arr[3]);
		mtx[i+4] = GFMul(0x09, arr[0]) ^ GFMul(0x0e, arr[1]) 
			^ GFMul(0x0b, arr[2]) ^ GFMul(0x0d, arr[3]);
		mtx[i+8] = GFMul(0x0d, arr[0]) ^ GFMul(0x09, arr[1]) 
			^ GFMul(0x0e, arr[2]) ^ GFMul(0x0b, arr[3]);
		mtx[i+12] = GFMul(0x0b, arr[0]) ^ GFMul(0x0d, arr[1]) 
			^ GFMul(0x09, arr[2]) ^ GFMul(0x0e, arr[3]);
	}
}

str AES::encrypt(str message) {
	if (message.length() != 128) throw "message length error";
	state in;
	for (int i = 0; i < 16; i++)
		in[i] = byte(message.substr(i * 8, 8));

	unsigned round = 0;
	AddRoundKey(in, round);

	for(++round; round < NR; ++round) {
		SubBytes(in);
		ShiftRows(in);
		MixColumns(in);
		for(int i = 0; i < 4; ++i)
			key[i] = w[4 * round + i];
		AddRoundKey(in, round);
	}

	SubBytes(in);
	ShiftRows(in);
	AddRoundKey(in, round);
	str result;
	for (int i = 0; i < 16; i++)
		result += in[i].to_string();
	return result;
}

str AES::decrypt(str message) {
	if (message.length() != 128) throw "message length error";
	state in;
	for (int i = 0; i < 16; i++)
		in[i] = byte(message.substr(i * 8, 8));

	unsigned round = NR;
	AddRoundKey(in, round);

	for(--round; round > 0; --round) {
		InvShiftRows(in);
		InvSubBytes(in);
		AddRoundKey(in, round);
		InvMixColumns(in);
	}

	InvShiftRows(in);
	InvSubBytes(in);
	AddRoundKey(in, round);
	str result;
	for (int i = 0; i < 16; i++)
		result += in[i].to_string();
	return result;
}

str AES::getKey() {
	return KEY;
}