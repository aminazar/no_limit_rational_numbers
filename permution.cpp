#include "permution.h"

 permution_node::permution_node(unsigned u)
 {//if(!u){cout<<"Can't add";abort();}
  n=u;next=this;}

 permution_node::permution_node(unsigned u,nodp before)
 {n=u;next=before->next;}

 //Find f(before) in permution
 unsigned permution_node::find(unsigned before, nodp &t)
 {
  t=this;
  do{if(t->next->n==before)return t->next->next->n;
     t=t->next;}while(t!=this);
  return 0;
 }

 permution::permution(){n=0;np=0;next=0;}

 permution::permution(unsigned i,unsigned j)
 {
  n=(i<j)?j:i;
  next=0;
  np=new permution_node(i);
  np->next=new permution_node(j,np);
 }

permution::permution(const permution &p)
 {
  permution_node *t=p.np,*s=np;
  if(t)s=np=new permution_node(t->n);

  t=t->next;
  while(t!=p.np)
  {s->next=new permution_node(t->n,s);
   s=s->next;
   t=t->next;}

  if(p.next)next=new permution(*(p.next));
  else next=0;
 }

 permution::~permution()
 {
  if(np)
  {permution_node *t=np,*tt;
   do
   {tt=t->next;
    delete t;
    t=tt;}while(t!=np);
  }
  if(next)delete next;
 }

 permution *permution::find(unsigned i,permution_node::nodp &p)
 {
  if(i>n){p=0;return 0;}
  permution *t=this;
  unsigned f;
  while(t)
  {if(t->np)
   {f=t->np->find(i,p);
    if(f)return t;}
   t=t->next;}

  p=0;
  return 0;
 }

bool permution::is_redundant()
 {
  if(!np)return true;
  if(np==np->next)return true;
  return false;
 }

void permution::erase_redundant()
 {
  permution *cur=this,*t=next;
  while(t)
  {while(t->is_redundant())
   {cur->next=t->next;
    t->next=0;
    delete t;
    t=cur->next;}
   cur=t;t=t->next;}

  if(is_redundant())
  {if(next)
   {np=next->np;
    next->np=0;
    t=next;
    next=next->next;
    t->next=0;
    delete t;}
   else if(np){delete np;np=0;}
  }
 }

unsigned permution::operator()(unsigned i)
 {
  if(i>n)return i;
  permution *t=this;
  unsigned f;
  while(t)
  {if(t->np)
   {permution_node *p;
    f=t->np->find(i,p);
    if(f)return f;}
   t=t->next;}
  return i;
 }

void permution::swap(unsigned i, unsigned j)
{
  if(i==j||!i||!j)return;
  if(i>n)n=i;
  if(j>n)n=j;

  permution_node *ifound,*jfound;
  permution *icycle=find(i,ifound),*jcycle=find(j,jfound);
  if(!ifound&&!jfound)
  {if(!np)
   {np=new permution_node(i);
    np->next=new permution_node(j,np);}
   else
   {permution *t=this;
    while(t->next)t=t->next;
    t->next=new permution(i,j);}
  }
  else if(ifound&&!jfound)ifound->next=new permution_node(j,ifound);
  else if(jfound&&!ifound)jfound->next=new permution_node(i,jfound);
  else
  {//swap ifound and jfound nexts
   permution_node *t=ifound->next;
   ifound->next=jfound->next;
   jfound->next=t;

   if(icycle==jcycle)
   //Then we will have a new cycle that must be connected
   {permution_node *trash;

    bool it_is_i=true;
    if(icycle->np->find(i,trash))it_is_i=false;//it means jcycle is not connected

    jcycle=this;
    while(jcycle->next)jcycle=jcycle->next;
    jcycle->next=new permution();
    jcycle->next->np=(it_is_i)?jfound:ifound;
   }
   else
   //Then two cycles have been joined, and there is two pointer to one cycle
   {if(icycle==this)jcycle->np=0;
    else icycle->np=0;}

   erase_redundant();
  }
 }

void permution::print()
 {
  if(np)
  {permution_node *t=np;
   cout<<"\n[";
   do
   {cout<<(t->n);
    t=t->next;
    if(t!=np)cout<<',';
    else cout<<']';}while(t!=np);
  }
  if(next)next->print();
 }

permution &permution::operator=(const permution &p)
 {
  if(np)
  {permution_node *t=np,*tt;
   do
   {tt=t->next;
    delete t;
    t=tt;}while(t!=np);
  }
  permution *pt;
  if(next)delete next;

  permution_node *t=p.np,*s=np;
  if(t)s=np=new permution_node(t->n);

  t=t->next;
  while(t!=p.np)
  {s->next=new permution_node(t->n,s);
   s=s->next;
   t=t->next;}

  if(p.next)next=new permution(*(p.next));
  else next=0;
 }

permution permution::operator*=(permution p)
 {
  permution *perp=&p;
  do
  {
   permution_node *pern=perp->np;
   if(pern)do
   {pern=pern->next;
    if(pern)swap(perp->np->n,pern->n);}while(pern!=perp->np);
   perp=perp->next;}while(perp);
  return *this;
 }

permution permution::operator*(permution &p)
 {
  permution temp=*this;
  return temp*=p;
 }
