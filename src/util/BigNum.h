#pragma once
#include<iostream> 
#include<string> 
#include<iomanip>
#include<cstring>
#include<algorithm> 

class BigNum { 
private: 
    int len;
public:
	int a[500];
    BigNum(){ len = 1;memset(a,0,sizeof(a)); }
    BigNum(const int);
    BigNum(const char*);
    BigNum(const BigNum &);
    BigNum &operator=(const BigNum &);
    
    friend std::istream& operator>>(std::istream&,  BigNum&);
    friend std::ostream& operator<<(std::ostream&,  BigNum&);
    
    BigNum operator+(const BigNum &) const;
    BigNum operator-(const BigNum &) const; 
    BigNum operator*(const BigNum &) const;
    BigNum operator/(const int   &) const;
    
    int operator%(const int &) const;
    BigNum operator % (const BigNum &) const;
    bool operator>(const BigNum & T)const;
    bool operator>(const int & t)const;
    bool operator == (const BigNum& he)const;
    
    std::string print() const;
};