#include "BigNum.h"

#define MAXN 9999
#define MAXSIZE 10
#define DLEN 4

BigNum::BigNum(const int b) {
    int c,d = b;
    len = 0;
    memset(a,0,sizeof(a));
    while(d > MAXN)
    {
        c = d - (d / (MAXN + 1)) * (MAXN + 1); 
        d = d / (MAXN + 1);
        a[len++] = c;
    }
    a[len++] = d;
}
BigNum::BigNum(const char*s) {
    int t,k,index,l,i;
    memset(a,0,sizeof(a));
    l=strlen(s);   
    len=l/DLEN;
    if(l%DLEN)
        len++;
    index=0;
    for(i=l-1;i>=0;i-=DLEN)
    {
        t=0;
        k=i-DLEN+1;
        if(k<0)
            k=0;
        for(int j=k;j<=i;j++)
            t=t*10+s[j]-'0';
        a[index++]=t;
    }
}
BigNum::BigNum(const BigNum & T) : len(T.len) { 
    int i; 
    memset(a,0,sizeof(a)); 
    for(i = 0 ; i < len ; i++)
        a[i] = T.a[i]; 
} 
BigNum & BigNum::operator=(const BigNum & n) {
    int i;
    len = n.len;
    memset(a,0,sizeof(a)); 
    for(i = 0 ; i < len ; i++) 
        a[i] = n.a[i]; 
    return *this; 
}
std::istream& operator>>(std::istream & in,  BigNum & b) {
    char ch[MAXSIZE*4];
    int i = -1;
    in>>ch;
    int l=strlen(ch);
    int count=0,sum=0;
    for(i=l-1;i>=0;)
    {
        sum = 0;
        int t=1;
        for(int j=0;j<4&&i>=0;j++,i--,t*=10)
        {
            sum+=(ch[i]-'0')*t;
        }
        b.a[count]=sum;
        count++;
    }
    b.len =count++;
    return in;
    
}
std::ostream& operator<<(std::ostream& out,  BigNum& b) {
    int i;  
    out << b.a[b.len - 1]; 
    for(i = b.len - 2 ; i >= 0 ; i--) {
        out.width(DLEN); 
        out.fill('0'); 
        out << b.a[i]; 
    } 
    return out;
}

BigNum BigNum::operator+(const BigNum & T) const {
    BigNum t(*this);
    int i,big;      //位数   
    big = T.len > len ? T.len : len; 
    for(i = 0 ; i < big ; i++) 
    { 
        t.a[i] +=T.a[i]; 
        if(t.a[i] > MAXN) 
        { 
            t.a[i + 1]++; 
            t.a[i] -=MAXN+1; 
        } 
    } 
    if(t.a[big] != 0)
        t.len = big + 1; 
    else
        t.len = big;   
    return t;
}
BigNum BigNum::operator-(const BigNum & T) const {  
    int i,j,big;
    bool flag;
    BigNum t1,t2;
    if(*this>T)
    {
        t1=*this;
        t2=T;
        flag=0;
    }
    else
    {
        t1=T;
        t2=*this;
        flag=1;
    }
    big=t1.len;
    for(i = 0 ; i < big ; i++)
    {
        if(t1.a[i] < t2.a[i])
        { 
            j = i + 1; 
            while(t1.a[j] == 0)
                j++; 
            t1.a[j--]--; 
            while(j > i)
                t1.a[j--] += MAXN;
            t1.a[i] += MAXN + 1 - t2.a[i]; 
        } 
        else
            t1.a[i] -= t2.a[i];
    }
    t1.len = big;
    while(t1.a[t1.len - 1] == 0 && t1.len > 1)
    {
        t1.len--; 
        big--;
    }
    if(flag)
        t1.a[big-1]=0-t1.a[big-1];
    return t1; 
} 

BigNum BigNum::operator*(const BigNum & T) const { 
    BigNum ret; 
    int i,j,up; 
    int temp,temp1;   
    for(i = 0 ; i < len ; i++)
    { 
        up = 0; 
        for(j = 0 ; j < T.len ; j++)
        { 
            temp = a[i] * T.a[j] + ret.a[i + j] + up; 
            if(temp > MAXN)
            { 
                temp1 = temp - temp / (MAXN + 1) * (MAXN + 1); 
                up = temp / (MAXN + 1); 
                ret.a[i + j] = temp1; 
            } 
            else
            { 
                up = 0; 
                ret.a[i + j] = temp; 
            } 
        } 
        if(up != 0) 
            ret.a[i + j] = up; 
    } 
    ret.len = i + j; 
    while(ret.a[ret.len - 1] == 0 && ret.len > 1)
        ret.len--; 
    return ret; 
} 
BigNum BigNum::operator/(const int & b) const { 
    BigNum ret; 
    int i,down = 0;   
    for(i = len - 1 ; i >= 0 ; i--)
    { 
        ret.a[i] = (a[i] + down * (MAXN + 1)) / b; 
        down = a[i] + down * (MAXN + 1) - ret.a[i] * b; 
    } 
    ret.len = len; 
    while(ret.a[ret.len - 1] == 0 && ret.len > 1)
        ret.len--; 
    return ret; 
}
int BigNum::operator % (const int & b) const {
    int i,d=0;
    for (i = len-1; i>=0; i--) {
        d = ((d * (MAXN+1))% b + a[i])% b;
    }
    return d;
}
BigNum BigNum::operator % (const BigNum& he) const {
    BigNum m(*this);
    while (m > he or m == he) m = m - he;
    while (BigNum(0) > m) m = m + he;
    return m; 
}
bool BigNum::operator>(const BigNum & T) const { 
    int ln; 
    if(len > T.len)
        return true; 
    else if(len == T.len)
    { 
        ln = len - 1; 
        while(a[ln] == T.a[ln] && ln >= 0)
            ln--; 
        if(ln >= 0 && a[ln] > T.a[ln])
            return true; 
        else
            return false; 
    } 
    else
        return false; 
}
bool BigNum::operator >(const int & t) const {
    BigNum b(t);
    return *this>b;
}
bool BigNum::operator == (const BigNum& he)const {
	if (len != he.len) return false;
	for (int i = 0; i < len; i++)
		if (a[i] != he.a[i]) return false;
	return true;
}

std::string BigNum::print() const { 
    int i; std::ostringstream o;   
    o << a[len - 1]; 
    for(i = len - 2 ; i >= 0 ; i--)
    { 
        o.width(DLEN); 
        o.fill('0'); 
        o << a[i]; 
    }
    return o.str();
}