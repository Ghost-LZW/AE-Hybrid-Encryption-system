#include "random/rand.ipp"
#include "aes/aes.h"
#include "sha3/keccak.ipp"
#include "ecc/py_ecc.h"
#include "util/CBC.h"
#include <iostream>
#include <string>
#include <cstdio>
#include <algorithm>
#include <utility>
#include <fstream>

using std::string;
using std::cout;
using std::endl;
using std::ofstream;
using std::ifstream;
using std::cerr;
using std::bitset;
using std::cin;

void help() {
	puts("aee混合加密系统\n-h\t显示帮助\n-g\t随机生成密钥对\n-e\t进行加密\t后方添加文件地址\n-d \t进行解密\t后方添加文件地址\n");
}

void gen() {
	Random<2> rand;
	string Private = rand.next();
	int N = stoi(Private, nullptr, 16);
	int opt;
	cerr << "1. 输入公共点 X Y 2.随机公共点 X Y 请输入序号:" << endl;
	string X, Y;
	cin >> opt;
	if (opt == 1) {
		cin >> X >> Y;
	} else {
		X = rand.next();
		Y = rand.next();
	}
	int A = 5, B = 13;
	std::pair<string, string> Pkey = ECC::getKey(N, X, Y, A, B);
	std::cerr << "您的私钥为: " << Private << endl;
	std::cerr << "您的公钥为: " << Pkey.first << "|" << Pkey.second << endl;
	if (opt == 2) {
		std::cerr << "公共点为: " << X << ' ' << Y << endl;
	}
	ofstream outfile("cryption.txt", std::ios::out);
	outfile << "Private: " << Private << '\n';
	outfile << "Public (X|Y): " << Pkey.first + "|" + Pkey.second << '\n';
	outfile << "A: " << A << '\n';
	outfile << "B: " << B << '\n';
	outfile.close();
}

void encrypt(const char* file) {
	ifstream fin(file, std::ios::in | std::ios::binary);
	if (!fin) {
		std::cerr << "Failed to open the file!" << endl;
		exit(-1);
	}
	string message;
	string vec = Util::HexToBinary(Random<16>().next());
	char t;
	while (fin.get(t)) {
		message += bitset<8>(t).to_string();
	}
	fin.close();
	string aesKey;
	message = CBC::encrypt(vec, message, aesKey);
	Keccak<> keccak;
	string sha3 = keccak.encrypt(message);

	cerr << "输入ECC参数: N, X, Y" << endl;
	string N, X, Y;
	cin >> N >> X >> Y;
	aesKey = ECC::encrypt(aesKey, stoi(N, nullptr, 16), X, Y, 5, 13);

	int len = message.length();
	string result;
	for (int i = 0; i < len; i += 8)
		result += char(bitset<8>(message.substr(i, 8)).to_ulong());
	string name(file);
	ofstream fout(name.data(), std::ios::out | std::ios::binary);
	for (const char& i : result)
		fout.put(i);
	fout.close();

	vec = Util::BinaryToHex(vec);
	std::cerr << "VI is " << vec << endl;
	std::cerr << "SHA3 is " << sha3 << endl;
	std::cerr << "AES Key " << aesKey << endl;

	fout.open("INFO.txt", std::ios::out);
	fout << "VI: " << vec << endl;
	fout << "SHA3: " << sha3 << endl;
	fout << "AES(Encrypted): " << aesKey << endl;
	fout.close();
}

void decrypt(const char* file) {
	ifstream fin(file, std::ios::in | std::ios::binary);
	if (!fin) {
		std::cerr << "Failed to open the file!" << endl;
		exit(-1);
	}

	string message;
	char t;
	while (fin.get(t)) {
		message += bitset<8>(t).to_string();
	}

	string vec, aesKey;
	cerr << "输入向量VI" << endl;
	cin >> vec;
	vec = Util::HexToBinary(vec);
	cerr << "输入AESKey" << endl;
	cin >> aesKey;
	cerr << "输入ECC参数, N, X, Y" << endl;
	string N, X, Y;
	cin >> N >> X >> Y;
	int Nres = stoi(N, nullptr, 16);
	aesKey = ECC::decrypt(aesKey, Nres, X, Y, 5, 13);
	try {
		message = CBC::decrypt(vec, message, aesKey);
	} catch (char const* error) {
		cerr << error << endl;
		exit(-1);
	}

	int len = message.length();
	string result;
	for (int i = 0; i < len; i += 8)
		result += char(bitset<8>(message.substr(i, 8)).to_ulong());
	string name(file);
	
	if (name.substr(name.length() - 4) == ".cry")
		name = name.substr(0, name.length() - 4);

	ofstream fout(name.data(), std::ios::out | std::ios::binary);
	for (const char& i : result)
		fout.put(i);
	fout.close();
}

int main(int argc, char const *argv[]) {
	if (argc == 1) help(); 
	else {
		for (int i = 1; i < argc; i++) {
			if (argv[i][0] == '-') {
				if (argv[i][1] == 'g')
					gen();
				else if (argv[i][1] == 'e')
					encrypt(argv[i + 1]);
				else if (argv[i][1] == 'd')
					decrypt(argv[i + 1]);
				else help();
			}
		}
	}

	return 0;
}