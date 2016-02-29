#ifndef __LARGENUM_H
#define __LARGENUM_H

#include <iostream>
#include <string>
#include <math.h>
#include <time.h>
#include "bvector.h"

using namespace std;

class number
{
 public:
 //Constructors
 number();
 explicit number(const string&);
 explicit number(string const&, counter &);
 number(long double);
 void random(counter );
 void show();
 //Assign
 number &operator=(const number &);

 //Add and Subtract
 number operator+(number const &) const;
 void operator+=(number const &);
 number operator-(number const &) const;
 void operator-=(number const &);
 number operator++();
 number operator--();

 //Multiply and Divide
 friend number operator*(number const &,number const &);
 void operator*=(number const &);
 number operator/(number const &) const;
 void operator/=(number const &);
 number operator%(number const &) const;
 void operator%=(number const &);
 number operator^(number const &) const;
 void operator^=(number const &);

 //Bit operators
 void operator<<=(counter );
 number operator<<(counter ) const;
 void operator>>=(counter );
 number operator>>(counter ) const;
 counter real_bits() const;
 bool bit_num(counter const &) const;
 void bit_num_set(counter const &,bool);


 //Comparing Operators
 bool operator<(number const &) const;
 bool operator>(number const &) const;
 bool operator==(number const &) const;
 bool operator!=(number const &) const;
 bool operator<=(number const &) const;
 bool operator>=(number const &) const;

 //Convertions
 operator long double() const;
 string to_string(bool=false) const;

 //Auxiliary friend functions
 friend number ni_mul(const number &a, const number &b);
 friend void divide(const number &a,const number &b,number &q,number &r);

 //Mathematical functions
 number log(number);
 number sqrt() const;
 number fact();
 number find_pow(const number&,number &)const;

 //etc
 bool is_zero() const;
 counter size() const;

 //Static datas
 //static counter precision;
 static counter screen_size;
 static block trash;
 static const number Zero,One,Ten;

 private:
 //Access
 block &at(counter bn);
 block ats(counter bn) const;

 //Not Used Bits in last block
 byte nub() const;

 //Auxiliary conjugate for optimizing multiply algorithm
 number conjugate() const;

 //Fit in minimum blocks
 void fit();

 //Error handler
 bool show_error(push_error &,bool=false);
 bool show_error(const number &) const;
 void show_error(byte);

 //etc
 void push_back(block=0);
 void block_shift_left (counter);
 void block_shift_right(counter);

 //Private data
 vect v;
};
#endif
