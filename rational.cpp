#include <string>
#include <iostream>
#include "rational.h"
#include "number.h"

using namespace std;

const byte BLOCK_BIT_SIZE=sizeof(block)*8;
const two_blocks NEXT_BIT=(two_blocks)1<<BLOCK_BIT_SIZE;
extern const char *decimalStr;

const number rational::nZero;
const number rational::nOne(1);
const number rational::nTen(10);
bool rational::digest=false;
byte rational::mode=0;
counter rational::precision=200;
counter rational::trash;


bool XOR(bool x,bool y){return ((x||y) && !(x&&y));}

number gcd(number const &x,number const &y);     //Greatest common divisor

counter rational::size() const
{return (s.real_bits()+f.real_bits()+m.real_bits());}

void rational::simplify()
{
 number d,q,r;
 divide(s,m,q,r);
 if(r.is_zero()){f+=q;s=nZero;m=nOne;}
 else
 {d=gcd(m,r);
  f+=q;
  s=r/d;m/=d;}

 if(s.is_zero()&&f.is_zero()){sign=0;m=nOne;}
}

void rational::round(unsigned log2epsilon)
{
    number makh(1),n,i,j,top;
    makh<<=log2epsilon;
    if(m<makh)return;
    n=m/makh;i=(s>n)?s-n:number::Zero;top=s+n;
    cout<<"search width: "<<n<<"\n";
    makh=1;
    for(;i<top;++i)
    {
        n=gcd(i,m);
        if(makh<n){makh=n;j=i;}
    }
    if(makh==number::One)return;
    cout<<"GGCD found:"<<makh<<" lost of precision:"<<rational((s<j)?(j-s):(s-j),m,number::Zero,(j<s))<<"\n";
    s=j/makh;m/=makh;
}

number rational::round(number &pr, byte insist_mode)
{
 if(pr==nZero)pr=nOne<<257;
 if(!s.is_zero())
 {
  pr=m/pr;
  if(pr>m-s)pr=m-s;
  if(pr.is_zero())return nZero;
  if(pr>number(50)&&insist_mode)
  {if(insist_mode==1)pr=number(50);
   else ; }
  else return nZero;

  number d,tm,ts,max=nOne;
  int mi,mj;

  for(int i=0;i<pr;i++)
   for(int j=0;j<pr;j++)
    if(i||j)
    {/*tm=(i<0)?(m-number(-i)*nOne):(m+number(i));
     ts=(j<0)?(s-number(-j)*nOne):(s+number(j));*/
     d=gcd(m+number(i),s+number(j));
     if(d>max){max=d;mi=i;mj=j;}}

  if(max!=nOne)
   {tm=(mi<0)?(m-number(-mi)*nOne):(m+number(mi));
    ts=(mj<0)?(s-number(-mj)*nOne):(s+number(mj));
    s=ts/max;m=tm/max;}
  return max;
  /*else
  {m>>=1;s>>=1;
   number d=gcd(m,s);
   if(d!=nOne){s/=d;m/=d;}}*/
 }
}

void rational::zero_divide() const
{cout<<"Rational: Divide by zero ERROR, terminated.";exit(1);}

rational::rational(){s=nZero;m=nOne;f=nZero;sign=false;}

rational::rational(const number & S,const number & M,bool Sign,const number & F)
{
 //cout<<"\nS="<<S<<" M="<<M<<" F="<<F;
 //getch();
 if(M.is_zero())zero_divide();
 sign=Sign;
 if(M==nOne&&F==nZero){f=S;m=M;}
 else
 {s=S;m=M;f=F;
  simplify();}
}

rational::rational(long double F)
{
 sign=(F<0);
 if(sign)F=-F;
 f=F;
 m=nOne;
 long double c=fmod(F,1);
 while(c)
 {
  m*=NEXT_BIT;
  s<<=BLOCK_BIT_SIZE;
  s+=c*NEXT_BIT;
  c=fmod(c*NEXT_BIT,1);
 }
 simplify();
}

void rational::set(const number & S,const number & M,bool Sign,const number & F)
{
 if(M.is_zero())zero_divide();
 sign=Sign;
 if(M==nOne&&F==nZero){f=S;m=M;s=0;}
 else{s=S;m=M;f=F;simplify();}
}

void rational::set(const string &in, counter &pos)
{
 size_t point,slash,minus;
 unsigned char brkchr[]={',',';',']',' ',9,'\n','\0'};
 string input;
 const string *p=&in;

 for(unsigned i=0;brkchr[i];i++)
 {size_t found=p->find_first_of(brkchr[i]);
  if(found!=string::npos){input=p->substr(0,found);p=&input;}}
  if(p==&in)input=in;

 minus = input.find_first_of('-');
 if(minus!=string::npos)
 {input=input.substr(minus+1);
  pos+=minus+1;
  size_t parenthesis=input.find('(');
  if(parenthesis!=string::npos)
  {input=input.substr(parenthesis+1);
   pos+=parenthesis+1;}
 }
  //cout<<"\ninput="<<input<<endl;

 point = input.find_first_of('.');
 slash = input.find_first_of('/');

 number w(input,pos);

 if(point==string::npos && slash==string::npos)
  set(w, nOne,minus!=string::npos );

 else if(point!=string::npos)
  {
     pos++;
   counter P=0;
   number y(input.substr(point+1),P);
   set(y, nTen^number(P), minus!=string::npos&&minus<point, w);
   pos+=P;}

 else
  {point = input.find_first_of("+");

   if(point!=string::npos && point<slash)
   {pos+=2;
    number y(input.substr(point+1),pos);
    number z(input.substr(slash+1),pos);
    set(y, z, minus!=string::npos&&minus<point, w);}

   else
   {pos++;
    number y(input.substr(slash+1),pos);
    set(w, y, minus!=string::npos&&minus<slash, nZero);}
  }
}

rational operator-(rational const &num)
{
 rational temp=num;
 if(!temp.s.is_zero()||!temp.f.is_zero())temp.sign = !num.sign;
 return temp;
}

bool rational::operator==(rational const &num)const
{return (f==num.f&&s==num.s&&m==num.m&&sign==num.sign);}

bool rational::operator<(rational const &num)const
{
 //if(*this==num)return false;
 if(sign&&!num.sign)return true;
 if(!sign&&num.sign)return false;
 bool temp=(f!=num.f) ? (f<num.f) : (s*num.m<num.s*m);
 if(sign)return !temp;
 return temp;
}

bool rational::operator<=(rational const &num)const
{return (*this<num || *this==num);}

bool rational::operator>(rational const &num)const
{return (num < *this);}

bool rational::operator>=(rational const &num)const
{return (num <= *this);}

bool rational::operator!=(rational const &num)const
{return !(num == *this);}

void rational::operator+=(rational const &num)
{
 if(!XOR(sign,num.sign))
 {
  f+=num.f;
  s=s*num.m;
  s+=m*num.s;
  m=m*num.m;
  simplify();
 }

 else if(sign)
 *this = num - (-*this);
 else
 *this -= (-num);
}


void rational::operator-=(rational const &num)
{
 if(!XOR(sign,num.sign))
 {
  number S1=s*num.m,S2=num.s*m;
  bool fsign,ssign;
  m*=num.m;
  if(f<num.f){(f=num.f-f);fsign=true;}
         else{(f-=num.f);fsign=false;}
  if(S1<S2){s=S2-S1;ssign=true;}
       else{s=S1-S2;ssign=false;}
  if(XOR(fsign,ssign))
  {if(!f.is_zero()){--f;s=m-s;sign=XOR(sign,fsign);}
               else sign=XOR(sign,ssign);}
  else
   sign=XOR(sign,fsign);
  simplify();
 }
 else if(sign)
 {sign=0; *this = (*this)+num;if(!f.is_zero()||!s.is_zero())sign=!sign;}
 else
 *this +=(-num);
}

 void rational::operator*=(rational const &num)
{*this = *this * num;}

 void rational::operator/=(rational const &num)
{*this = *this / num;}

 void rational::operator^=(const number &exp)
{*this = *this ^ exp;}


rational rational::operator+(rational const &num)const
{
 rational temp;
 if(!XOR(sign,num.sign))
 {
  temp.f=f+num.f;
  temp.s=s*num.m+m*num.s;
  temp.m=m*num.m;
  temp.sign=sign;
  temp.simplify();
  return temp;
 }

 else if(sign)return num - (-*this) ;
         else return *this - (-num);
}

rational rational::operator-(rational const &num)const
{
 if(!sign&&!num.sign)
 if(!XOR(sign,num.sign))
 {rational temp;
  number S1=s*num.m,S2=num.s*m;
  bool fsign,ssign;
  temp.m=m*num.m;

  if(f<num.f){temp.f=num.f-f;fsign=true;}
         else{temp.f=f-num.f;fsign=false;}

  if(S1<S2){temp.s=S2-S1;ssign=true;}
       else{temp.s=S1-S2;ssign=false;}

  if(XOR(fsign,ssign))
  {if(!temp.f.is_zero()){--temp.f;temp.s=temp.m-temp.s;temp.sign=XOR(sign,fsign);}
                    else temp.sign=XOR(sign,ssign);}
  else
   temp.sign=XOR(sign,fsign);

  temp.simplify();
  return temp;
 }
 else if(sign) return -((-*this)+num);
               return *this+(-num);

}

rational rational::operator*(rational const &num)const
{
 rational temp(f*num.f);
 //number A=gcd(s,num.m),B=gcd(m,num.s),C=gcd(f,num.m),D=gcd(num.f,m);

 //temp = temp + rational((f/C)*num.s,num.m/C)+
 //        rational((num.f/D)*s,m/D)+
 //        rational((s/A)*(num.s/B),(m/B)*(num.m/A));

 temp += rational(f*num.s,num.m)+rational(num.f*s,m)+rational(s*num.s,m*num.m);

 if(!(temp.s.is_zero()&&temp.f.is_zero()))temp.sign=XOR(sign,num.sign);

 return temp;
}

rational rational::operator/(rational const &num)const
{
 if(num.f==nZero && num.s==nZero)zero_divide();
 if(*this==num)return rational(nOne);
 number S=f*m+s,
        M=num.f*num.m+num.s,
        A=gcd(S,M) , B=gcd(m,num.m),
        C=gcd(S,m) , D=gcd(M,num.m);

 rational temp((S/(C*A))*(num.m/(B*D)),
               (M/(A*D))*(m/(B*C)),
                XOR(sign,num.sign));

 if(temp.s.is_zero()&&temp.f.is_zero())temp.sign=false;
 //rational temp(num.m*(f*m+s),m*(num.f*num.m+num.s),xor(sign,num.sign));
 return temp;
}

rational rational::operator^(const number &exp)const
{
 if(exp.is_zero())return rational(nOne);
 return rational((f*m+s)^exp,m^exp);
}

number rational::to_number()const
{return f;}

rational::operator long double()const
{
 long double x=(long double)f+ (long double)s/m;
 if(sign)x*=-1;
 return x;
}

/*
rational::operator bool() const
{
 return !(f.is_zero()&&s.is_zero());
} */

rational rational::sqrt()const
{
 rational ret(nOne),Two(2);
 if(sign){cout<<"Sqrt error";exit(-1);}
 if(f==nZero)return inverse().sqrt().inverse();
 else if(f==nOne||f==nZero)ret=rational(nOne);
 else ret=rational(f.sqrt());
 for(int i=0;i<3;i++)
 {//cout<<"\nret="<<ret;
  ret=(ret+*this/ret)/Two;}

 return ret;
}

rational rational::inverse() const
{
 rational inv;
 inv.sign=sign;
 if(!f.is_zero())inv.m=(s+f*m);
 else inv.m=s;
 inv.s=m;
 inv.simplify();
 return inv;
}

ostream &operator<<(ostream &out,const rational &num)
{
 return out<<num.to_string();
 /*if(num.sign){out<<"-";if(!(num.s.is_zero()||num.f.is_zero()))out<<"(";}
 if(!num.f.is_zero())out<<(num.f.to_string(rational::digest));
 if(!num.s.is_zero()){if(!num.f.is_zero())out<<"+";out<<(num.s.to_string(rational::digest))<<"/"<<(num.m.to_string(rational::digest));}
 if(num.sign&&!num.s.is_zero()&&!num.f.is_zero())out<<")";
 if(num.f.is_zero()&&num.s.is_zero())out<<0;
 return out;*/
}

string rational::to_string() const
{
 if(!mode||mode>2)
 {
 string out;
 if(digest)number::screen_size=(precision-5)/3;
 bool plus=!s.is_zero()&&!f.is_zero(),
      parenthesis=sign&&plus,
      z=f.is_zero()&&s.is_zero();
 if(z)out=string("0");
 else
 {if(sign)out=string("-");
  if(parenthesis)out+=string("(");
  if(!f.is_zero())out+=f.to_string(digest);
  if(plus)out+=string("+");
  if(!s.is_zero()){out+=s.to_string(digest);
                   out+=string("/");
                   out+=m.to_string(digest);}
  if(parenthesis)out+=string(")");
 }
 return out;
 }

 else if(mode==1)
 //ten base without period
 {
 string r;
 if(sign)r=string("-");
 r+=f.to_string(digest);
 if(r.find("e")!=string::npos)return r;

 if(!s.is_zero())
 {r+=string(".");
  rational t(s,m),exp(nTen^(precision-r.length()));
  counter c=0;
  t*=exp;
  string tf=t.f.to_string();

  if(f.is_zero()&&!t.f.is_zero())
  {counter e=(tf.length()<precision-r.length())?
  				 (precision-r.length()-tf.length()+1):1;
   number ex(e);
   counter p=tf.length();
   while(tf[--p]=='0'||tf.length()+3>precision)tf.erase(p);
   if(tf.length()>1)tf.insert(1,".");
   if(sign)tf.insert(0,"-");
   tf+=string("e-");
   tf+=ex.to_string();
   return tf;}

  while(tf.length()<precision-r.length())tf=string("0")+tf;

  r+=tf;
 }
 return r;
 }

 else
 //ten base with period
 {
  string r;
  if(sign)r=string("-");
  r=r+f.to_string(digest);
  if(r.find_first_of('e')!=string::npos)return r;
  if(!s.is_zero())
  {number five(5),two(2),e5,e2,ft;
   rational t(s,m),zero;
   (m.find_pow(five,e5)).find_pow(two,e2);

   e2+=e5;
   r+=string(".");

   counter c=0;
   bool tanavob=false;

   unsigned p=precision-r.length();
   while(t!=zero&&c<p)
   {t*=rational(10);
    if(tanavob)if(ft==t.s){r+=")";break;}
    if(c==e2){p--;r+="(";ft=t.s;tanavob=true;}
    //cout<<ch;
    r+=string(1,*(decimalStr+(counter)t.f));
    c++;
    t.f=nZero;}
   }

  return r;
 }
}



istream &operator>>(istream &in, rational &num)
{
 string input;
 in>>input;
 num.set(input);
 return in;
}

number gcd_(number x,number y)
{
 while(!y.is_zero()){number t=y;y=x%y;x=t;}
 //if(x.is_zero())return rational::nOne;
 return x;
}

number gcd(number const &x,number const &y)
{
 if(y<=x)return gcd_(x,y);
 return gcd_(y,x);
}

//}//end of namespace in the file
