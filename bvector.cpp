#include <malloc.h>
#include "bvector.h"

using namespace std;

const unsigned BYTES_PER_BLOCK=512;
const unsigned B_SIZE=BYTES_PER_BLOCK/sizeof(block);

memory_block::memory_block()
 {
  m = (block *)malloc(BYTES_PER_BLOCK);
  if(!m)throw byte(1);
  ba++;
  n=b=0;
  bf=bl=0;
 }

memory_block::memory_block(memory_block *before)
 {
  m = (block *)malloc(BYTES_PER_BLOCK);
  if(!m)throw byte(2);
  ba++;
  b=before;
  n=0;
  bf=bl=0;
 }

memory_block::memory_block(const memory_block &a)
{
  m = (block *)malloc(BYTES_PER_BLOCK);
  if(!m)throw byte(3);
  ba++;
  b=n=0;
  bf=a.bf;
  bl=a.bl;
  for(block_counter i=bf;i<bl;i++)m[i]=a.m[i];
}

void memory_block::operator=(const memory_block &a)
{
  bf=a.bf;
  bl=a.bl;
  for(block_counter i=bf;i<bl;i++)m[i]=a.m[i];
}

memory_block::~memory_block()
{free(m);bfr++;}

bool memory_block::push_back(block x)
{
  if(bl==B_SIZE)return false;
  m[bl++]=x;
  return true;
}

bool memory_block::push_front(block x)
{
  if(!bf)
   if(bl>bf)return false;
   else bf=bl=B_SIZE;
  m[--bf]=x;
  return true;
}

bool memory_block::pop_back()
{
  if(bl==bf+1)return false;
  bl--;
  return true;
}

bool memory_block::pop_front()
{
  if(bl==bf+1)return false;
  bf++;
  return true;
}

void memory_block::show()
{
  cout<<"\n----------------------\n";
  cout<<"Before="<<b<<endl;
  for(counter i=0;i<B_SIZE;i++)
  {bool inb=(i>=bf&&i<bl);
   if(inb)cout<<'(';
   cout<<hex<<m[i];
   if(inb)cout<<") "; else cout<<' ';}
  cout<<"\nCurrent="<<this<<"\nAfter="<<n;
}

 vect::vect()
 {cn=cp=s=0;cur=last=first=0;}

 void vect::init_pointer(bool infirst)
 {
  if(infirst)
  {if(first)
    {cur=first;cp=first->bf;cn=0;}
   else
    {cur=0;cn=cp=0;}}
  else
  {if(last)
    {cur=last;cp=last->bl-1;cn=(s+first->bf-1)/B_SIZE;}
   else
    {cur=0;cn=cp=0;}}
 }

 vect::vect(const vect &a)
 {
  s=a.s;
  memory_block *t=a.first,*u=0, **p=&first;

  if(!t)first=last=0;
  while(t)
  {
   try{*p=new memory_block(*t);}
   catch(byte err){throw;}
   (*p)->b=u;
   u=*p;
   p=&((*p)->n);
   t=t->n;
   if(!t)(*p)=0;
  }
  last=u;
  init_pointer();
 }

 vect &vect::operator=(const vect &a)
 {
  if(&a==this)return *this;

  s=a.s;
  if(!s)free();
  else
  {
   memory_block *t=a.first,**p=&first,*u=0;

   try
   {
   while((*p)&&t)
   {**p=*t;t=t->n;u=*p;p=&((*p)->n);}

   if(!(*p))
   {
    while(t)
    {
     *p=new memory_block(*t);
     (*p)->b=u;
     u=*p;
     p=&((*p)->n);
     t=t->n;
     if(!t)(*p)=0;
    }
    last=u;
   }
   else
   {
    while(last!=u){t=last->b;if(t)t->n=0;delete last;last=t;}
    if(!last)last=first=0;
   }
   }
   catch(byte err){throw;}
   init_pointer();
  }
  return *this;
 }

 vect::~vect()
 {
  memory_block *t=first;
  while(t)
  {t=first->n;
   delete first;
   first=t;}
 }

 void vect::free()
 {
  cn=cp=s=0;
  memory_block *t=first;
  while(t)
  {t=first->n;
   delete first;
   first=t;}
  first=last=cur=0;
 }
 
 counter vect::size() const
 {return s;}

 bool vect::empty() const
 {return !s;}

 block *vect::next()
 {
  if(!cur)return 0;
  block *r=cur->m+cp;
  cp++;
  if(cn*B_SIZE+cp-first->bf>=s){cur=0;cn=s;}
  else if(!(cp%B_SIZE)){cp=0;cur=cur->n;cn++;}
  return r;
 }

 block *vect::before()
 {
  if(!cur)return 0;
  block *r=cur->m+cp;
  if(cp)cp--;
  else if(cn){cp=B_SIZE-1;cur=cur->b;cn--;}
  else{cur=0;cn=s;}
  return r;
 }

 block vect::ats(counter i) const
 {
  counter B=(i+first->bf)/B_SIZE,R=(i+first->bf)%B_SIZE;
  /*2010:Not compatible with const
  if(B==cn)return *(cur->m+R);
  else
  {*/
   memory_block *t=first;
   for(register counter b=0;b<B;b++)
    t=t->n;
   /*Not compatible with const
   cur=t;
   cn=B;
    */
   return *(t->m+R);
  //}
 }

 block &vect::operator[](counter i)
 {
  counter B=(i+first->bf)/B_SIZE,R=(i+first->bf)%B_SIZE;
  if(B==cn)return *(cur->m+R);
  else
  {
   memory_block *t=first;
   for(register counter b=0;b<B;b++)
    t=t->n;
   cur=t;
   cn=B;
   return *(t->m+R);
  }
 }

 block &vect::at(counter i)
 {
  if(i>=s)throw byte(4);
  counter B=(i+first->bf)/B_SIZE,R=(i+first->bf)%B_SIZE;
  if(cn==B)return *(cur->m+R);
  else
  {
   memory_block *t=first;
   for(register counter b=0;b<B;b++)
    t=t->n;
   cur=t;
   cn=B;
   return *(t->m+R);
  }
 }

 block &vect::back() const
 {return *(last->m+last->bl-1);}

 block &vect::front()
 {return *(first->m+first->bf);}

 void vect::push_back(block x)
 {
  try
  {
   if(!first)cur=last=first=new memory_block;
   if(!last->push_back(x))
   {
    last->n = new memory_block(last);
    last=last->n;
    last->push_back(x);
   }
   s++;
  }
  catch(byte i)
  {push_error err;
   err.size_of_vector=s;
   err.cannot_pushed=x;
   err.error_occured=i;
   throw err;}
 }

 void vect::push_front(block x)
 {
  try
  {
   if(!first)cur=last=first=new memory_block;
   if(!first->push_front(x))
   {
    first->b = new memory_block();
    first->b->n=first;
    first=first->b;
    first->push_front(x);
    init_pointer();
   }
   s++;
  }
  catch(byte i)
  {push_error err;
   err.size_of_vector=s;
   err.cannot_pushed=x;
   err.error_occured=i;
   throw err;}
 }

 void vect::pop_back()
 {
  if(s)
  {if(!last->pop_back())
   {last=last->b;
    if(last)
    {delete last->n;
     last->n=0;}
    else
     {delete first;first=last=0;}}
   s--;}
 }

 void vect::pop_front()
 {
  if(s)
  {if(!first->pop_front())
   {first=first->n;
    if(first)
    {delete first->b;
     first->b=0;}
    else
     {delete last;first=last=0;}
     init_pointer();}
   s--;}
 }

void vect::show()
 {
  cout<<"\nSize="<<s;
  cout<<"\nfirst="<<first<<" last="<<last<<endl;
  memory_block *t=first;
  while(t)
  {if(t==first)cout<<"FIRST BLOCK\n";
   if(t==last)cout<<"LAST BLOCK\n";
   t->show();
   //getch();
   t=t->n;}
 }