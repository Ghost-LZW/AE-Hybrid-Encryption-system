

main : aes.o util.o pyecc.o CBC.o
	g++ ./src/main.cc -o aee aes.o util.o py_ecc.o CBC.o -std=c++14 -Wall -lpython38 -L"./src/ecc/" -O2

test : aes.o util.o test.o
	g++ -o main main.o aes.o util.O2 -std=c++14

ecc : BigNum.o ecc.o
	g++ -o ecc ecc.o BigNum.o

ecc.o :
	g++ -c ./src/ecc/ecc.cc -std=c++14

test.o :
	g++ -c ./src/test/main.cc

aes.o :
	g++ -c ./src/aes/aes.cc -std=c++14

util.o :
	g++ -c ./src/util/util.cc  -std=c++14

BigNum.o :
	g++ -c ./src/util/BigNum.cc

CBC.o : aes.o
	g++ -c ./src/util/CBC.cc -std=c++14

pyecc.o : util.o
	g++ -c ./src/ecc/py_ecc.cc -Wall -lpython38 -L"./src/ecc/" -I./src/ecc/include -O2 -std=c++14 -lstdc++
