#ifndef __RATIONAL_H
#define __RATIONAL_H
#include <iostream>
#include <stdlib.h>
#include <ctype.h>
#include "number.h"

class rational									//Rational number class
{
 number f,s,m; bool sign;            //Number is f+(s/m), so that s<m.
 void simplify();            //Simplifies number so gcd(s,m)=1,s<m.
 void zero_divide() const;

 public:
 const static number nOne,nZero,nTen;
 static bool digest;
 static counter precision,trash;
 static byte mode;

 rational();
 explicit rational(const number &S,const number &M=nOne,bool Sign=false,const number &F=nZero); //Constructors
 explicit rational(long double F);
 void set(const number &S,const number &M=nOne,bool Sign=false,const number &F=nZero);
 void set(const string&,counter &p=trash);
 number round(number&,byte=0);
 void round(unsigned log2epsilon);
 friend rational operator-(rational const &num);
 bool operator==(rational const &num) const;
 bool operator<(rational const &num) const;
 bool operator<=(rational const &num) const;
 bool operator>(rational const &num) const;
 bool operator>=(rational const &num) const;
 bool operator!=(rational const &num) const;

 void operator+=(rational const &num);
 void operator-=(rational const &num);
 void operator*=(rational const &num);
 void operator/=(rational const &num);
 void operator^=(const number &exp);

 rational operator+(rational const &num) const;
 rational operator-(rational const &num) const;
 rational operator*(rational const &num) const;
 rational operator/(rational const &num) const;
 rational operator^(const number &exp) const;
 counter size()const;

 number to_number() const;
 operator long double() const;
 string to_string() const;

 rational sqrt() const;
 rational inverse() const;

 friend ostream &operator<<(ostream &out,const rational &num);
 friend istream &operator>>(istream &in, rational &num);
};
//}
#endif