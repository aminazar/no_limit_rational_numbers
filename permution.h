#ifndef __PERMUTION_H
#define __PERMUTION_H
#include <iostream>

using namespace std;

class permution_node
{
 unsigned n;
 permution_node *next;
 typedef permution_node* nodp;
 public:
 permution_node(unsigned u);

 permution_node(unsigned u,nodp before);

 //Find f(before) in permution
 unsigned find(unsigned before, nodp &t);

 friend class permution;
};

class permution
{
 permution_node *np;
 permution *next;

 public:
 unsigned n;
 permution();
 permution(unsigned i,unsigned j);
 permution(const permution &p);
 ~permution();
 permution *find(unsigned i,permution_node::nodp &p);
 bool is_redundant();
 void erase_redundant();
 unsigned operator()(unsigned i);
 void swap(unsigned i, unsigned j);
 void print();
 permution &operator=(const permution &p);
 permution operator*=(permution p);
 permution operator*(permution &p);
};
#endif
