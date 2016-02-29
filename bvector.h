#ifndef __VECTOR_H
#define __VECTOR_H

#include <iostream>

typedef unsigned block;
typedef unsigned long counter;
typedef unsigned block_counter;
typedef unsigned long two_blocks;
typedef long signed_block;
typedef unsigned char byte;

class push_error	//For exception handling
{
 public:
 block cannot_pushed;
 counter size_of_vector;
 byte error_occured;
};

class memory_block
{
 memory_block *n,*b;
 block* m;
 block_counter bl,bf;

 public:
 static counter ba,bfr;
 memory_block();

 memory_block(memory_block *before);

 memory_block(const memory_block &a);

 void operator=(const memory_block &a);


 ~memory_block();

 bool push_back(block x);

 bool push_front(block x);

 bool pop_back();

 bool pop_front();

 void show();

 friend class vect;
};

class vect
{
 memory_block *first,*last,*cur;
 counter s,cn;
 block_counter cp;

 public:
 vect();
 void init_pointer(bool infirst=true);

 vect(const vect &a);

 vect &operator=(const vect &a);

 ~vect();

 void free();

 counter size() const;

 bool empty() const;

 block *next();

 block *before();

 block &operator[](counter i);

 block ats(counter i) const;

 block &at(counter i);

 block &back() const;

 block &front();

 void push_back(block x);

 void push_front(block x);

 void pop_back();

 void pop_front();

 void show();
};
#endif
