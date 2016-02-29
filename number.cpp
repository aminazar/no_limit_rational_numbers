#include <iostream>
#include <cstring>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include "number.h"                                          
#define min(a,b) (a<b) ? a:b
#define max(a,b) (a<b) ? b:a

using namespace std;

const byte BIT_PER_BYTE=8;
const byte BLOCK_SIZE=sizeof(block);
const byte LONG_SIZE=sizeof(long);
const byte BLOCK_BIT_SIZE=BLOCK_SIZE*BIT_PER_BYTE;
const block LAST_BIT=1<<(BLOCK_BIT_SIZE-1);
const two_blocks NEXT_BIT=(two_blocks)1<<BLOCK_BIT_SIZE;
const block FULL_BLOCK=NEXT_BIT-1;
const char *decimalStr="0123456789";

counter number::screen_size=20;
block number::trash;

const number number::One(1);
const number number::Zero(0);
const number number::Ten(10);

long double BASE_TEN=log(2)/log(10);
counter memory_block::ba=0,memory_block::bfr=0;

/////////////////////////
// Trivial definitions //
/////////////////////////
number::number(){}

bool number::is_zero() const
{return v.empty();}

///////////////////////
// Private functions //
///////////////////////
void number::push_back(block x)
{
 try{v.push_back(x);}
 catch(push_error err)
 {
  if(show_error(err,true))push_back(x);
  else throw;
 }
}

block number::ats(counter bn) const
{
 if(bn>=v.size())return 0;
 return v.ats(bn);
}

block &number::at(counter bn)
{
 counter bc=0;
 try{while(bn>=v.size()){bc++;push_back(0);}}
 catch(push_error err)
 {
  while(bc--)v.pop_back();
  trash=0;
  return trash;}

 return v[bn];
}

counter number::size() const
{
 //if(!v.empty())if(!v.back())fit(); //Not compatible with cosnt
 return v.size();
}

bool number::bit_num(counter const &n) const
{return (ats(n/BLOCK_BIT_SIZE)&(1<<(n%BLOCK_BIT_SIZE)));}

void number::bit_num_set(counter const &n,bool b)
{if(b)at(n/BLOCK_BIT_SIZE)|=(1<<(n%BLOCK_BIT_SIZE));
 else at(n/BLOCK_BIT_SIZE)&=(FULL_BLOCK-(1<<(n%BLOCK_BIT_SIZE)));}


number number::conjugate() const
{
 counter c;
 number temp;

 for(counter bit=size()*BLOCK_BIT_SIZE;bit;bit--)
 {c=0;
  while(bit&&bit_num(bit-1)){c++;bit--;}
  if(!bit)break;
  if(c>=3)temp.bit_num_set(bit,true);}

 return temp;
}
void number::show()
{
 cout<<"---size="<<size()<<"---"<<endl;
 //cout<<"Real bit size: "<<real_bits()<<endl;
 for(counter i=0;i<v.size();i+=4)
 {for(int j=0;j+i<v.size()&&j<4;j++)
  {cout<<"v["<<dec<<(v.size()-(i+j)-1)<<"]=";
   cout.width(4);
   cout<<hex<<unsigned(v[v.size()-(i+j)-1])<<char(9);}
  cout<<endl;}
// cout<<"Integer value="<<*this<<endl;
 cout<<"----------"<<dec<<endl;
}

void number::fit()
{
 while(!v.empty()){if(v.back())break;v.pop_back();}
}

void number::block_shift_right(counter blocks)
{
 for(counter i=0;i<blocks;i++)v.pop_front();
}

void number::block_shift_left(counter blocks)
{
 try {for(counter i=0;i<blocks;i++)v.push_front(0);}
 catch(push_error err){show_error(err);}
}

byte number::nub() const
{
 if(v.empty())return 0;

 block filter=LAST_BIT;

 byte r=0;
 //Warning: in current code everywhere nub() used after size(), and if
 //there is need to fit(), size do it. Because returning a correct number
 //of non-used-bits needs to fit() number, by changing code, you may have
 //to reactivate below code, which is not active now for optimization.
 //if(!v.back())fit();
 while(!(v.back()&filter)&&filter){r++;filter>>=1;}
 return r;
}

counter number::real_bits() const
{return (size()*BLOCK_BIT_SIZE) - nub();}

bool number::show_error(push_error &err,bool retry)
{
 cout<<"\nLong Number Error Handler: ";
 cout<<"I can not push a new block to a number that has current size of ";
 cout<<err.size_of_vector<<" blocks. Value of new block is "<<unsigned(err.cannot_pushed);
 cout<<".\nThe error occured because memory allocator couldn't ";
 switch(err.error_occured)
 {
  case 1:cout<<"initialize first memory block.";
         break;
  case 2:cout<<"extend to a new memory block.";
         break;
  case 3:cout<<"construct a copy of a memory block.";
 }

 bool false_answer;
 do
 {
  if(!retry)cout<<"\n\nAbort or Ignore?";
  else cout<<"\n\nAbort, Ignore or Retry?";
  char ans;
  cin>>ans;
  ans=tolower(ans);
  false_answer=false;
  switch(ans)
  {
   case 'a':exit(0);
   case 'i':cout<<"\nWarning: Ignoring this problem will cause severe calculation errors.\n";
            //getch();
            return false;
   case 'r':if(retry)return true;
    default:false_answer=true;
  }
 }while(false_answer);
}

bool number::show_error(const number &n) const
{
 cout<<"\nLong Number Error Handler: ";
 cout<<"Subtract error for trying subtract a larger number from a smaller one.";
 cout<<"\nFirst number:"<<to_string(true);
 cout<<"\nSecenod number:"<<n.to_string(true);
 bool false_answer;
 cout<<"\n\nWarning: Ignoring this problem will cause saving result as a positive number.";
 do
 {
  cout<<"\nAbort or Ignore?";
  char ans;
  cin>>ans;
  ans=tolower(ans);
  false_answer=false;
  switch(ans)
  {
   case 'a':abort();
   case 'i':
            //getch();
            return true;
    default:false_answer=true;
  }
 }while(false_answer);
}

void number::show_error(byte err)
{
 cout<<"\nLong Number Error Handler: Memory allocation failed, ";
 switch(err)
 {
  case 1: cout<<"so I can not initialize first memory block.";break;
  case 2: cout<<"so I can not extend current memory block to next one.";break;
  case 3: cout<<"so I can not make copy of a memory block.";break;
  default: cout<<"caused unknown error.";
 }
 cout<<"\n\nAdvise: If you should save many numbers in memory, try a smaller memory block.\n";
 //getch();
 abort();
}

////////////////
//Constructors//
////////////////
number::number(long double q)
{
 block b;
 while(q>=1)
 {
  q/= NEXT_BIT;
  b = (q-floor(q))*NEXT_BIT;
  (q>=1)?push_back(b) : push_back(q*NEXT_BIT);
 }
}

number::number(string const &str)
{
 counter l=0;
 number exp;
 while(l<str.length()&&isdigit(str[l]))l++;
 if(str.length()>l)if(tolower(str[l])=='e')exp=number(str.substr(l+1));

 if(l)
 {number x=One;
  for(counter i=0;i<l;i++,x*=Ten)
   {if(str[l-i-1]!=48)*this+=x*number(str[l-i-1]-48);}}
 if(!exp.is_zero())*this*=(Ten^exp);
}

number::number(string const &str,counter &pos)
{
 counter l=0;
 number exp;
 while(l<str.length()&&isdigit(str[l]))l++;
 if(str.length()>l)if(tolower(str[l])=='e')exp=number(str.substr(l+1));

 if(l)
 {number x=One;
  for(counter i=0;i<l;i++,x*=Ten)
   {if(str[l-i-1]!=48)*this+=x*number(str[l-i-1]-48);}}

 if(!exp.is_zero())*this*=(Ten^exp);
 pos+=l;
}

void number::random(counter max_blocks)
{
 if(!v.empty())v.free();
 if(!(rand()%7))srand(clock());

 counter s=rand()%max_blocks;
 for(counter i=0;i<s;i++)
  v.push_back((block)rand()<<(rand()%2));
}
//////////////////////
//    Operations     /
//////////////////////
number &number::operator=(const number &n)
{
 try{v=n.v;}
 catch(byte err){show_error(err);}
}

number number::operator+(number const &n) const
{
 number temp;

 bool cary=false;
 counter m = max(size(),n.size());

 for(counter i=0;i<m;i++)
 {two_blocks t=(two_blocks)cary+ats(i)+n.ats(i);
  temp.at(i)=t%NEXT_BIT;
  cary = t>FULL_BLOCK;}
 if(cary)temp.push_back(1);

 return temp;
}

void number::operator+=(number const &n)
{
 bool cary=false;
 counter m = n.size();

 for(counter i=0;i<m||cary;i++)
 {
  two_blocks t=(two_blocks)cary+ats(i)+n.ats(i);
  at(i)=t%NEXT_BIT;
  cary = t>FULL_BLOCK;}
 if(cary)push_back(1);
}

number number::operator++(){*this+=One;return *this;}

number number::operator-(number const &n) const
{
 if(*this<n)if(show_error(n))return n-*this;
 number temp;
 if(*this==n)return temp;
 signed_block cary=false;
 counter m=max(size(),n.size());
 /*cout<<"\nMINUS SHOW\n"<<"\n-------------First show";
 v.show();
 cout<<"\n\n-------------Next show";
 n.v.show();*/
 for(counter i=0;i<m;i++)
 {signed_block t=(signed_block)ats(i)-n.ats(i)-cary;
  cary=(t<0);
  temp.at(i)=(cary) ? (t+NEXT_BIT) : t;}

 temp.fit();
 return temp;
}

void number::operator-=(number const &n)
{
 if(*this<n)if(show_error(n))*this=n-*this;
 if(*this==n){v.free();return;}
 signed_block cary=false;
 counter m=n.size();
 for(counter i=0;i<m||cary;i++)
 {signed_block t=(signed_block)ats(i)-n.ats(i)-cary;
  cary=(t<0);
  at(i)=(cary) ? (t+NEXT_BIT) : t;}

 fit();
}

number number::operator--(){*this-=One;return *this;}

void number::operator<<=(counter u)
{
 if(!u)return;

 if(size())
 {
  counter blocks= u/BLOCK_BIT_SIZE;
  byte bits=u%BLOCK_BIT_SIZE;

  if(nub()<bits)
   v.push_back(0);

  block filter=((1<<bits)-1)<<(BLOCK_BIT_SIZE-bits),
        temp=0;

  v.init_pointer();
  block *ptr;
  while(ptr=v.next())
  {block x=*ptr;
   *ptr=((*ptr)<<bits)|temp;
   temp=(x&filter)>>(BLOCK_BIT_SIZE-bits);}

  if(blocks)block_shift_left(blocks);
 }
}

number number::operator<<(counter u) const
{number temp=*this;temp<<=u;return temp;}

number ni_mul(const number &a,const number &b)
{
 number temp;
 counter rb = b.real_bits(),
 			bit = 0;
 for(;bit<rb;bit++)if(b.bit_num(bit))temp+=(a<<bit);

 return temp;
}

number operator*(const number &a, const number &b)
{
 const number *less, *more;
 number temp;

 if(a.v.size()<b.v.size()){less=&a;more=&b;}
 else{less=&b;more=&a;}

 if(less->is_zero())return temp;

 number con=less->conjugate();

 if(!con.is_zero())
  temp=(*more)*(*less+con)-ni_mul(*more,con);
 else
  temp=ni_mul(*more,*less);

 return temp;
}

void number::operator*=(number const &n)
{*this=*this*n;}

void number::operator>>=(counter u)
{
 if(u>=real_bits())
 {v.free();return;}

 counter blocks = u/BLOCK_BIT_SIZE;
    byte bits   = u%BLOCK_BIT_SIZE;

 if(size())
 {
  if(blocks)block_shift_right(blocks);

  if(bits&&size())
  {
   block filter=(1<<bits)-1,
         temp=0;


   /*for(counter i=size();i;i--)
   {block x=v[i-1];
    v[i-1]=(v[i-1]>>bits)|temp;
    temp=(x&filter)<<(BLOCK_BIT_SIZE-bits);}*/

   v.init_pointer(false);
   block *ptr;
   while(ptr=v.before())
   {block x=*ptr;
    *ptr=((*ptr)>>bits)|temp;
    temp=(x&filter)<<(BLOCK_BIT_SIZE-bits);}
  }
 }
 fit();
}

number number::operator>>(counter u) const
{number temp=*this;temp>>=u;return temp;}

number number::operator^(number const &n) const
{
 number /*temp=*this,*/ret=One;
 for(number i;i<n;++i)ret*=*this;
 //counter nr=n.real_bits();
 //for(counter i=0;i<nr;i++)
 //{if(n.bit_num(i))ret*=temp;
 // temp*=temp;}

 return ret;
}

void number::operator^=(number const &n)
{
 number temp=*this;
 *this=One;
 counter nr=n.real_bits();
 for(counter i=0;i<nr;++i)
 {if(n.bit_num(i))*this*=temp;
  temp*=temp;}
}

void divide(const number &a,const number &b,number &q,number &r)
{
 if(b.is_zero()){cout<<"\nDivide by zero.\n";return;}
 q.v.free();
 r=a;
 if(a<b)return;

 counter m=a.real_bits(),
         n=b.real_bits(),
         bitq=m-n,
         bita=m-n;
 r>>=bitq;

 bool first=false;

 try{while(true)
 {
  do
  {
   if((r<b)&&bita)
   {
    r<<=1;
    if(a.bit_num(--bita)){if(r.v.empty())r.v.push_back(1);else r.v.front()++;}
    if(!first)q.bit_num_set(bitq--,false);
    first=false;}
   else break;
  }while(bita);
  if(!bita&&r<b)break;
  r-=b;
  q.bit_num_set(bitq--,true);
  first=true;
 }}
 catch(push_error err){r.show_error(err);}
 if(bitq)q>>=(bitq+1);
 //if(bitq){cout<<"\nq="<<q<<" bitq="<<bitq;getch();}
}

number number::operator/(number const &n) const
{
 number t1,t2;

 divide(*this,n,t1,t2);
 return t1;
}

void number::operator/=(number const &n)
{
 number t1,t2;
 divide(*this,n,t1,t2);
 *this=t1;
}

number number::operator%(number const &b) const
{
 if(b.is_zero()){cout<<"\nDivide by zero.\n";return 0;}
 if(*this<b)return *this;



 counter m=real_bits(),
         n=b.real_bits(),
         bita=m-n;

 number r=*this>>bita;

 while(true)
 {
  do
  {if((r<b)&&bita)
   {r<<=1;
    if(bit_num(--bita)){if(r.v.empty())r.v.push_back(1);else r.v.front()++;}}
   else break;
  }while(bita);

  if(!bita&&r<b)break;
  r-=b;
 }
 return r;
}

void number::operator%=(number const &b)
{*this=*this%b;}

////////////////////
//   Comparison   //
////////////////////
bool number::operator<(number const &n) const
{
 if(size()==n.size())
 {counter s=size();
  for(counter i=s;i;i--)
  {if(ats(i-1)>(n.ats(i-1)))return false;
   if(ats(i-1)<(n.ats(i-1)))return true;}
  return false;}

 return size()<n.size();
}

bool number::operator<=(number const &n) const
{
 if(size()==n.size())
 {counter s=size();
  for(counter i=s;i;i--)
  {if(ats(i-1)>n.ats(i-1))return false;
   if(ats(i-1)<n.ats(i-1))return true;}
  return true;}

 return size()<n.size();
}

bool number::operator==(number const &n) const
{
 if(size()==n.size())
 {counter s=size();
  for(counter i=s;i;i--)
   if(ats(i-1)!=n.ats(i-1))return false;
  return true;}
 return false;
}

bool number::operator>(number const &n) const
{return n<*this;}

bool number::operator!=(number const &n) const
{return !(*this==n);}

bool number::operator>=(number const &n) const
{return !(*this<n);}


////////////////
// Convertion //
////////////////
number::operator long double() const
{
 if(is_zero())return 0.0;
 long double temp=1.0,d=0;
 for(counter i=0;i<size();i++,temp*=NEXT_BIT)
  d+=temp*ats(i);
 return d;
}

string number::to_string(bool digest) const
{
 if(is_zero())return string("0");
 string x,e;
 number a=*this;

 if(digest)
 {
  counter exp=(real_bits()-1)*BASE_TEN+1,s=0,t=exp;
  if(exp<=screen_size)digest=false;
  else
  {
   while(t){t/=10;s++;}
   if(s>=screen_size-1)return string("...");
   else
   {exp-=(screen_size-s-1);
    a/=Ten^number(exp);
    t=exp;
    while(t)
    {e.insert(0,decimalStr+(t%10),1);
     t/=10;}
    e.insert(0,string("e"));
   }
  }
 }

 number LARGE(1e9);
 number b,c;

 do
 {a.fit();
  divide(a,LARGE,c,b);
  unsigned long w=b;

  for(unsigned i=0;i<9;i++)
  {x=string(1,*(decimalStr+(w%10)))+x;
   w/=10;
   if(c.is_zero()&&!w)break;
  }
  a=c;}while(!c.is_zero());

 if(digest)x+=e;
 return x;
}

////////////////////
// Math functions //
////////////////////
number number::sqrt() const
{
 number x=2,y;

 do
 {
  number y=*this/x;
  x=(x+*this/x)>>1;
  y=x;}while(*this<(x*x));
 while((y*y)<*this)++y;
 return y-One;
}

number number::log(number u)
{
 number i;
 for(;(u^i)<=(*this);++i);
 if(i.is_zero())cout<<"\nLog error cause:\n";
 return i-One;
}

number number::fact()
{
 number t=One;
 for(number i(2);i<=*this;++i)t*=i;
 return t;
}

number number::find_pow(const number &base,number &exp) const
{
 number temp=*this,q,r;
 exp.v.free();
 do{divide(temp,base,q,r);if(r.is_zero()){temp=q;++exp;}}while(r.is_zero());
 return temp;
}
