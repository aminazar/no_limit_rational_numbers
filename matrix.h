#ifndef __MATRIX_H
#define __MATRIX_H
#include "rational.h"

typedef rational entry;
extern rational Rzero,Rone;

class matrix
{
 entry **p;
 unsigned r,c;//Row and Columns
 public:

 //Constructors
 matrix();
 matrix(unsigned ,unsigned);
 matrix(const matrix &);
 matrix (const string &);

 //Destructor
 ~matrix();

 //Operators
 matrix &operator=(const matrix &);
 entry &operator()(unsigned i,unsigned j);

 //Basic Row Operations:
 //if j==0, Row Operation will be Divide 'i'th row by x,
 //if x==0, Row Operation will be Swap 'i' and 'j' rows,
 //and else Row Operation will be Subtract row 'j' multiplied by 'x' from row 'i'.
 void bro(unsigned i,unsigned j,entry x);

 bool hermite();//Change matrix to hermit form, returns 'true' as it possible,
                //otherwise 'false'.

 void show_error(unsigned i,bool termin=true);
 friend ostream &operator<<(ostream &,const matrix &);
 friend istream &operator>>(istream &,matrix &);
};
#endif
