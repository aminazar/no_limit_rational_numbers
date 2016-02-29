#include "rational.h"
#include "permution.h"
#include <fstream>

/////////////////////////////////
// Matrix Template Declaration //
/////////////////////////////////
template <class entry>
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

 //Dimensions
 unsigned rows() const;
 unsigned columns() const;

 //Operators
 matrix<entry> &operator=(const matrix &);
 entry &operator()(unsigned i,unsigned j)const;
 matrix<entry> operator*(const matrix<entry> &m)const;

 //Sub matrix
 matrix<entry> sub_matrix(unsigned sr=0,unsigned sc=0,unsigned er=0,unsigned ec=0) const;

 //Functions for solving linear equations system
 void bro(unsigned i,unsigned j,const entry &x);
 //Basic Row Operations:
 //if j==0, Row Operation will be Divide 'i'th row by x,
 //if x==0, Row Operation will be Swap 'i' and 'j' rows,
 //and else Row Operation will be Subtract row 'j' multiplied by 'x' from row 'i'.

 bool pivot(unsigned row, int t, bool fast_return, permution &perm);
 //if you turn 'fast_returning' on, if matrix would be singular, pivoting cancels. Otherwise, it continues.
 //Meaning of return value: When it is false, in case of fast returning shows singularity,
 //                         and otherwis, means that more pivoting of this type does not change matrix anymore.
 //Pivoting types are: 0 Gause-Jordan pivoting, leading to hermite form
 //						  1 Gause Elimination, leading to upper triangular form
 //						  2 Gause Elimination, partial pivoting
 //						  3 Gause Elimination, complete pivoting, swap columns index will be saved in 'perm'

 matrix<entry> backward_subst(permution &perm);
 //Backward substitution for upper triangular matrix, and returns result as nx1 matrix.
 //Permution of columns in case of complete pivoting will swaps enteries of result.

 bool hermite();
 //Change matrix to hermite form, returns 'true' if is'nt singular,
 //otherwise returns 'false'.

 void scale();

 void LU_factorize(matrix<entry> &L, matrix<entry> &U)const;

 matrix<entry> solve(int method) const;
 //Methodes:
 //0: Gause-Jordan
 //1: Gausian elimination-Regular pivoting
 //2: Gausian elimination-Partial pivoting
 //3: Gausian elimination-Complete pivoting

 void iterative_solve(matrix<entry> &prim, int method,unsigned iterations, entry eps);
 //Methodes:
 //0: Jacobi
 //1:Gause-Seidel

 void screen_display();

 static void show_error(unsigned i,bool termin=true);
 friend ostream &operator<<(ostream &,const matrix<entry> &);
 friend istream &operator>>(istream &,matrix<entry> &);
 friend class matrix_bro;
 static byte precision;
 static bool show;
};

byte matrix<float>::precision=10,
     matrix<double>::precision=10,
     matrix<long double>::precision=10,
     matrix<rational>::precision=10;
bool matrix<float>::show=true,
     matrix<double>::show=true,
     matrix<long double>::show=true,
     matrix<rational>::show=true;
//////////////////////////////////
// Global functions declaration //
//////////////////////////////////
template <class entry>
entry abs(const entry &x)
{if(x<entry(0))return -x;
 return x;}

template <class type1,class type2>
void copy(const matrix<type1> &m1, matrix<type2> &m2);

template <class type1,class type2>
void difference(matrix<type1> &m1, const matrix<type2> &m2,bool absolute=true);

template <class entry>
void wilkinson(matrix<entry> &w, unsigned r);

int choose_mem_location(const string &title, int &loaded,bool change)
{
 string cap[]={" (Empty)"," (Used)"},location_menu[4];
 string loc("Memory Location ");
 location_menu[3]=string("");
 int bit=1;
 for(unsigned i=0;i<3;i++,bit*=2)
 {location_menu[i]=loc+string(char(49+i));
  if(loaded&bit)
   location_menu[i]+=cap[1];
  else
   location_menu[i]+=cap[0];}
 while(true)
 {int i=centre_menu(location_menu,title);
  bit=1;for(int j=0;j<i;j++,bit*=2);
  if((loaded&bit))return i;
  else if(change){loaded|=bit;return i;}
 }
}

main()
{
 string main_menu[8]={"Matrix Input",
 				          "Matrix Output",
                		 "Solve As A Linear Equations System",
                 		 "Hermite Form",
                		 "L U Factorization",
                 		 "Change Memory Mode",
                 		 "Quit",""};
 string format_menu[5]={"Rational (Unlimited memory - Precise)",
 					         "Float (10 Bytes - 25 Signifcant digits)",
                        "Float (8 Bytes - 19 Significant digits)",
                        "Float (4 Bytes - 9 Significant digits)",""};
 matrix<rational> R[3];
 matrix<long double> LD[3];
 matrix<double> D[3];
 matrix<float> F[3];
 int format=1;
 int loaded[4]={0,0,0,0};

 while(true)
 {
  int c=centre_menu(main_menu,"Main Menu");
  //Quit
  if(c==6)
  {text_mode();
   cout<<"Exit?";if(tolower(getch())=='y')break;}

  //Input
  else if(!c)
  {
   string type_menu[]={"Get From Keyboard","Load From File","",""};
   int type=centre_menu(type_menu,"Choose Input Device:");
   int l=choose_mem_location("Choose a Location to save:",loaded[format],true);

   text_mode();
   print_centre("** Matrix Input **");

   if(type)
   {cout<<"\nEnter file name: ";cin>>type_menu[0];
    ifstream in(type_menu[0].c_str());
    if(!in)cout<<endl<<'\''<<type_menu[0]<<"' can not be openned.\n";
    else
    {in>>R[0];
     in.close();
     if(!R[0].rows())cout<<"\nNo useful data is in the file.";
     else cout<<"\nLoaded.";}
    cout<<"\nPress a key to continue...";
    getch();}
   else
   {cout<<"Enter matrix, Use MATLAB format:\n";
    cin>>R[0];}

   switch(format)
   {case 1:copy(R[0],LD[l]);break;
    case 2:copy(R[0],D[l]);break;
    case 3:copy(R[0],F[l]);}
  }

  //Output
  else if(c==1)
  {if(!loaded[format]){text_mode();cout<<"There is no matrix in this memory mode to send to output.";getch();}
   else
   {int l=choose_mem_location("Choose a matrix for output:",loaded[format],false);
    string type_menu[]={"Display on the Screen","Save in a File",""};
    int type=centre_menu(type_menu,"Choose Output Device:");

    text_mode();
    print_centre("** Matrix Output **");

    ofstream file;
    if(type)
    {cout<<"\nEnter file name: ";cin>>type_menu[0];
     file.open(type_menu[0].c_str(),ios::app);
     if(!file)cout<<endl<<'\''<<type_menu[0]<<"' can not be openned.\n";
    }

    if(!format)
    		  {if(type){file<<R[l];file.close();cout<<"\nSaved.";}
            else R[l].screen_display();}

    else if(format==1)
      	  {if(type){file<<LD[l];file.close();cout<<"\nSaved.";}
            else LD[l].screen_display();}

   else if(format==2)
     		  {if(type){file<<D[l];file.close();cout<<"\nSaved.";}
            else D[l].screen_display();}

   else if(format==3)
     		  {
            if(type){file<<F[l];file.close();cout<<"\nSaved.";}
            else F[l].screen_display();}


   cout<<"\nPress a key to continue...";getch();
   }
  }

  else if(c==2)
  {if(!loaded[format]){text_mode();cout<<"There is no matrix in this memory mode to solve.";getch();}
   else
   {int l1=choose_mem_location("Choose a matrix for solve:",loaded[format],false);
    string methodes[5]={"Elimination Methodes","Iterative Methodes","","",""};
    int m=centre_menu(methodes,"Choose a group of methodes:");
    if(!m)
    {methodes[0]="No";methodes[1]="Yes";methodes[2]="";
     int scl=centre_menu(methodes,"Scaling?");
     methodes[0]="Gause - Jordan";methodes[1]="Gause Elimination (GE)";
     methodes[2]="GE With Partial Pivoting";methodes[3]="GE With Complete Pivoting";
     m=centre_menu(methodes,"Choose a method:");
     int l2=choose_mem_location("Choose a memory location to save result:",loaded[format],true);
     switch(format)
     {case 0:R[l2]=R[l1];
     	       if(scl)R[l2].scale();
             R[l2]=R[l2].solve(m);
             break;
      case 1:LD[l2]=LD[l1];
             if(scl)LD[l2].scale();
             LD[l2]=LD[l2].solve(m);
             break;
      case 2:D[l2]=D[l1];
             if(scl)D[l2].scale();
             D[l2]=D[l2].solve(m);
             break;
      case 3:F[l2]=F[l1];
             if(scl)F[l2].scale();
             F[l2]=F[l2].solve(m);}
    }
    else//Iterative methodes
    {methodes[0]="Jacobi";methodes[1]="Gause - Seidel";methodes[2]="";
     m=centre_menu(methodes,"Choose a method:");
     int l2=choose_mem_location("Choose a memory location to save result:",loaded[format],true);
     text_mode();
     print_centre(string("** ")+methodes[m]+string(" Method **"));
     int itr;long double eps;matrix<rational> prim;
     cout<<"\nEnter number of iterations: ";cin>>itr;
     cout<<"\nEnter 'eps' maximum distance between two iterations to stop: ";cin>>eps;
     cout<<"\nEnter a primary solution:\n";cin>>prim;
     switch(format)
     {case 0:R[l1].iterative_solve(prim, m,itr, rational(eps));
     			 R[l2]=prim;
             break;
      case 1:copy(prim,LD[l2]);
             LD[l1].iterative_solve(LD[l2], m,itr, eps);
     			 break;
      case 2:copy(prim,D[l2]);
             D[l1].iterative_solve(D[l2], m,itr, double(eps));
     			 break;
      case 3:copy(prim,F[l2]);
             F[l1].iterative_solve(F[l2], m,itr, float(eps));}

    }
   }
  }
  else if(c==3)
  {if(!loaded[format]){text_mode();cout<<"There is no matrix in this memory mode to solve.";getch();}
   else
   {int l1=choose_mem_location("Choose a matrix to conversion to hermite form:",loaded[format],false);
    int l2=choose_mem_location("Choose a memory location to save result:",loaded[format],true);
    switch(format)
     {case 0:R[l2]=R[l1];
     			 R[l2].hermite();
             break;
      case 1:LD[l2]=LD[l1];LD[l2].hermite();
     			 break;
      case 2:D[l2]=D[l1];D[l2].hermite();
      		 break;
      case 3:F[l2]=F[l1];F[l2].hermite();}
   }
  }

  else if(c==4)
  {if(!loaded[format]){text_mode();cout<<"There is no matrix in this memory mode to factorize.";getch();}
   else
   {int l1=choose_mem_location("Choose a matrix to factorize:",loaded[format],false);
    int l2=choose_mem_location("Choose a memory location to save L factor:",loaded[format],true);
    int l3=choose_mem_location("Choose a memory location to U factor:",loaded[format],true);
    switch(format)
     {case 0:R[l1].LU_factorize(R[l2],R[l3]);
     			 break;
      case 1:LD[l1].LU_factorize(LD[l2],LD[l3]);
     			 break;
      case 2:D[l1].LU_factorize(D[l2],D[l3]);
      		 break;
      case 3:F[l1].LU_factorize(F[l2],F[l3]);}
   }
  }

  else if(c==5)
  {int f=centre_menu(format_menu,"Choose format:");
   format=f;
   int saveop=0;
   if(!format)
   {string save_options[]={"Rational Mode","Decimal Mode","Decimal Mode with Period",""};
    saveop=centre_menu(save_options,"Choose a Rational Output Mode:");}
    rational::mode=saveop;
   if((saveop>0&&!format)||format){text_mode();cout<<"\nEnter number of digits for decimal conversion: ";cin>>saveop;}
   if(saveop>=10){matrix<float>::precision=saveop;
                  matrix<double>::precision=saveop;
                  matrix<long double>::precision=saveop;
                  if(saveop>14)rational::precision=saveop;}
  }
 }
}


/////////////////////////////////
// Global functions definition //
/////////////////////////////////
void convert_string_to_numeric(string const &s, rational &a)
{a.set(s);}

void convert_string_to_numeric(string const &s,long double &a)
{a = atof(s.c_str());}

void convert_string_to_numeric(string const &s, double &a)
{a = atof(s.c_str());}

void convert_string_to_numeric(string const &s, float &a)
{a = atof(s.c_str());}

void convert_numeric_to_string(const float &a, string &s)
{char c[80];
 int x;matrix<double>::precision;
 s = string(gcvt(a,matrix<double>::precision,c));}

void convert_numeric_to_string(const double &a, string &s)
{
 char c[80];
 int x;matrix<double>::precision;
 s = string(gcvt(a,matrix<double>::precision,c));
}

void convert_numeric_to_string(const long double &a, string &s)
{
 char c[80];
 matrix<double>::precision;
 s = string(gcvt(a,matrix<double>::precision,c));
}

void convert_numeric_to_string(const rational &a, string &s)
{s = a.to_string();}

template <class type1,class type2>
void copy(const matrix<type1> &m1, matrix<type2> &m2)
{
 m2=matrix<type2>(m1.rows(),m1.columns());
 for(unsigned i=1;i<=m1.rows();i++)
  for(unsigned j=1;j<=m1.columns();j++)
   m2(i,j)=(type2)m1(i,j);
}

template <class type1,class type2>
void difference(matrix<type1> &m1, const matrix<type2> &m2,bool a)
{
 if(m1.rows()!=m2.rows()||m1.columns()!=m2.columns())
  matrix<type1>::show_error(5,0);
 else
 {for(unsigned i=1;i<=m1.rows();i++)
   for(unsigned j=1;j<=m1.columns();j++)
    if(a)m1(i,j) = abs(m1(i,j)-type1(m2(i,j)));
    else if(m1(i,j)!=type1(0))m1(i,j) = abs((m1(i,j)-type1(m2(i,j)))/m1(i,j));
 }
}

template <class entry>
void wilkinson(matrix<entry> &w,unsigned r)
{
 w=matrix<entry>(r,r+1);
 for(unsigned i=1;i<=w.rows();i++)
  for(unsigned j=1;j<=w.columns();j++)
  {if(i==j||j==w.columns())w(i,j)=entry(1);
   else if(j<i)w(i,j)=entry(-1);
   else w(i,j)=entry(0);}
}

////////////////////////////////
// Matrix Template Definition //
////////////////////////////////
template <class entry>
matrix<entry>::matrix(){p=0;r=c=0;}

template <class entry>
matrix<entry>::matrix(unsigned R,unsigned C)
{
 p=new entry*[R];
 if(!p)show_error(1,true);
 for(counter i=0;i<R;i++)
 {p[i]=new entry[C];if(!p[i])show_error(1,true);}
 r=R;c=C;
}

template <class entry>
matrix<entry>::matrix(const matrix &A)
{
 p=new entry*[A.r];
 if(!p)show_error(1,true);
 for(counter i=0;i<A.r;i++)
 {p[i]=new entry[A.c];
  if(!p[i])show_error(1,true);
  for(counter j=0;j<A.c;j++)
   p[i][j]=A.p[i][j];
 }
 r=A.r;c=A.c;
}

template <class entry>
matrix<entry>::matrix (const string &s)
{
 counter open=s.find('['),close,pos=0;
 char next_row = (s.find(';')==NPOS)?(10):(';'),
      next_col = (s.find(',')==NPOS)?(9):(',');

 if(open!=NPOS)
 {close=s.find(']');
  if(close==NPOS)show_error(7);
  pos=open+1;}

 counter tpos=pos,colon=s.find(next_row);
 counter C;

 r=0;c=0;
 while(tpos!=NPOS&&tpos<close)
 {C=0;r++;
  while(((tpos=s.find(next_col,tpos))!=NPOS)&&tpos<colon&&tpos<close){C++;tpos++;}

  if(colon!=NPOS)
  {tpos=colon+1;
   colon=s.find(next_row,tpos);}
  else tpos=NPOS;
  if(C && c && (C-c)){gotoxy(1,24);cout<<"Error in row:"<<(r+1);show_error(8,0);r=c=0;return;}
  if(!C&&c){if(r>1)r--;break;}
  c=C;}

 c++;
 tpos=pos;
 if(r)p=new entry*[r];
 if(!p)show_error(1,true);
 for(counter i=0;i<r;i++)
 {p[i]=new entry[c];
  if(!p[i])show_error(1,true);
  for(counter j=0;j<c;j++)
  {tpos = (j==c-1) ? (s.find(next_row,tpos)) : (s.find(next_col,tpos));
   convert_string_to_numeric(s.substr(pos,tpos),p[i][j]);
   pos=++tpos;}
 }
}

template <class entry>
matrix<entry>::~matrix()
{
 for(counter i=0;i<r;i++)delete[] p[i];
  delete[] p;
}

template <class entry>
unsigned matrix<entry>::rows()const
{return r;}

template <class entry>
unsigned matrix<entry>::columns()const
{return c;}

template <class entry>
matrix<entry> &matrix<entry>::operator=(const matrix &A)
{
 if(&A==this)return *this;
 if(p)
 {for(counter i=0;i<r;i++)delete[] p[i];
  delete[] p;}

 p=new entry*[A.r];
 if(!p)show_error(1,true);

 for(counter i=0;i<A.r;i++)
 {p[i]=new entry[A.c];
  if(!p[i])show_error(1,true);
  for(counter j=0;j<A.c;j++)
   p[i][j]=A.p[i][j];}
 r=A.r;c=A.c;
 return *this;
}

template <class entry>
entry &matrix<entry>::operator()(unsigned i,unsigned j)const
{if(!i||!j||i>r||j>c)show_error(2);
 return p[i-1][j-1];}

template<class entry>
void matrix<entry>::scale()
{
 entry max=entry(0);
 for(unsigned i=0;i<r;i++)
  for(unsigned j=0;j<c;j++)
   if(p[i][j]!=entry(0))
   {entry a=abs(p[i][j]);
    if(max<a)max=a;}
 if(max!=entry(1))
  for(unsigned i=0;i<r;i++)
   for(unsigned j=0;j<c;j++)
    if(p[i][j]!=entry(0))
     p[i][j]/=max;
}

template <class entry>
matrix<entry> matrix<entry>::operator*(const matrix<entry> &m)const
{
 if(c!=m.r){show_error(3,0);matrix<entry> a; return a;}
 matrix<entry> temp(r,m.c);

 for(unsigned i=0;i<r;i++)
  for(unsigned j=0;j<m.c;j++)
   temp.p[i][j]=entry(0);

 for(unsigned i=0;i<r;i++)
  for(unsigned j=0;j<m.c;j++)
   for(unsigned k=0;k<c;k++)
    temp.p[i][j]+=p[i][k]*m.p[k][j];
 return temp;
}


template <class entry>
void matrix<entry>::bro(unsigned i,unsigned j,const entry &x)
{
 if(show){text_mode();cout<<"Basic Row Operation: ";}
 unsigned t;
 if(!j)
 //Divide
 {if(show){cout<<"Row "<<j<<" divide by "<<x;getch();}
  if(x==entry(0)||i>r)show_error(6,true);
  else
  {int s=-1;
   for(t=0;t<c;t++)
   {if(p[i-1][t]!=entry(0))
    {if(&x==&(p[i-1][t]))s=t;
     else p[i-1][t]=p[i-1][t]/x;}}
   if(s>=0)p[i-1][s]=entry(1);
  }
 }

 else if(x==entry(0))
 //Swap
 {if(show){cout<<"Swap row "<<j<<" and "<<i;getch();}
  if(i>r||j>r||i==j)show_error(6,true);
  else for(t=0;t<c;t++){entry temp=p[i-1][t];p[i-1][t]=p[j-1][t];p[j-1][t]=temp;}}

 else
 //Subtract
 {if(show){cout<<"Subtract row "<<j<<" multiplied in "<<x<<" from row "<<i;getch();}
  if(i>r||j>r||i==j)show_error(6,true);
  else
  {int s=-1;
   for(t=0;t<c;t++)
   {if(&x==&(p[i-1][t]))s=t;
    else p[i-1][t]-=p[j-1][t]*x;}
   if(s>=0)p[i-1][s]-=p[j-1][s]*x;
  }
 }
 if(show){cout<<endl<<*this;getch();}
}

template <class entry>
bool matrix<entry>::hermite()
{
  permution null;
  bool ret=true;
  for(unsigned i=1;i<=r;i++)ret=ret&&pivot(i,0,false,null);
  return ret;
}

template<class entry>
matrix<entry> matrix<entry>::sub_matrix(unsigned sr,unsigned er,unsigned sc,unsigned ec)const
{
 if(!sr)sr=1;if(!sc)sc=1;if(!er)er=r;if(!ec)ec=c;
 if(sr>er||sc>ec){show_error(10,0);matrix<entry> x;return x;}
 matrix<entry> temp(er-sr+1,ec-sc+1);

 for(unsigned i=sr;i<=er;i++)
  for(unsigned j=sc;j<=ec;j++)
   temp.p[i-sr][j-sc]=p[i-1][j-1];

 return temp;
}

template <class entry>
bool matrix<entry>::pivot(unsigned row,int type, bool fast_return,permution &perm)
{
 if(!row||row>r||row>c){show_error(6,0);return false;}
 row--;

 unsigned col=row;
 if(type<2)
 {bool next_col=true;
  while(next_col&&col<c)
  {next_col=false;
   if(p[row][col]!=(entry)0){if(!type)bro(row+1,0,p[row][col]);next_col=false;}
   else if(row<r-1)
    for(unsigned j=row+1;j<r;j++)
    {if(p[j][col]!=(entry)0)
     {bro(row+1,col+1,(entry)0);
      if(!type)bro(row+1,0,p[row][col]);
      break;}
     if(j==r-1){if(fast_return)return false;col++;next_col=true;}
    }
   else {col++;next_col=true;}
  }
 }

 else if(type==2)
 {
  while(col<c)
  {
   unsigned max_index=row;
   for(unsigned i=row+1;i<r;i++)
    if(abs(p[max_index][col])<abs(p[i][col]))max_index=i;
   if(p[max_index][col]==entry(0))
   {if(fast_return)return false;
    col++;}
   else {if(max_index!=row)bro(max_index+1,row+1,entry(0));break;}
  }
 }

 else if(type==3)
 {
  unsigned maxi=row,maxj=row;
  for(unsigned i=row;i<r;i++)
   for(unsigned j=row;j<r;j++)
    if(abs(p[maxi][maxj])<abs(p[i][j])){maxi=i;maxj=j;}
  if(p[maxi][maxj]==entry(0))return false;
  else
  {if(maxi!=row)bro(maxi+1,row+1,entry(0));//Swap rows

   if(maxj!=row)
   //swaps columns j+1 and maxj+1,
   //and saves this swap in the permution data
   {perm.swap(row+1,maxj+1);
    for(unsigned i=0;i<r;i++)
    {entry temp=p[i][row];p[i][row]=p[i][maxj];p[i][maxj]=temp;}}
  }
 }

 if(col==c)return false;
 if(!type)
 {for(unsigned j=0;j<r;j++)
  if(row!=j)if(p[j][col]!=(entry)0)bro(j+1,row+1,p[j][col]);}
 else
 {if(row<r-1)
  for(unsigned j=row+1;j<r;j++)
   if(p[j][col]!=(entry)0)
    bro(j+1,row+1,p[j][col]/p[row][col]);}

 return true;
}

template <class entry>
matrix<entry> matrix<entry>::backward_subst(permution &perm)
{
 matrix<entry> ret(r,1);
 perm.inverse();
 for(unsigned i=r;i;i--)
 {ret(perm(i),1) = p[i-1][c-1];
  for(unsigned j=i+1;j<=r;j++)
   if(p[j-1][c-1]!=entry(0))
    ret.p[perm(i)-1][0]-=ret.p[perm(j)-1][0]*p[i-1][j-1];
  ret.p[perm(i)-1][0]=ret.p[perm(i)-1][0]/p[i-1][i-1];
 }
 return ret;
}


template <class entry>
matrix<entry> matrix<entry>::solve(int m)const
{
 matrix<entry> null;
 if(r+1!=c){show_error(9,0);return null;}
 matrix<entry> temp(*this);

 permution perm;
 for(unsigned i=1;i<=r;i++)
  if(!temp.pivot(i,m,true,perm)){show_error(4,0);return null;}

 if(!m)return temp.sub_matrix(0,0,c,c);
 else return temp.backward_subst(perm);
}

template <class entry>
void matrix<entry>::iterative_solve(matrix<entry> &prim, int method, unsigned iterations, entry eps)
{
 if(prim.r!=r||prim.c!=1){show_error(13,0);return;}
 if(r+1!=c){show_error(9,0);return;}

 for(unsigned i=0;i<r;i++)
  if(p[i][i]==entry(0))
   for(unsigned j=i+1;j<r;j++)
   {if(p[j][i]!=entry(0)){bro(i+1,j+1,entry(0));break;}
    if(j==r-1){show_error(4,0);return;}}

 matrix<entry> temp=prim;
 entry maxim=entry(1);

 for(unsigned i=0;i<iterations;i++)
 {
  if(show){cout<<"Iteration "<<i<<endl<<prim<<endl;getch();}
  for(unsigned j=0;j<r;j++)
  {temp.p[j][0] = p[j][c-1]/p[j][j];
   for(unsigned k=0;k<r;k++)
    if(j!=k)
    {if(!method||k>j)
      temp.p[j][0] -= (p[j][k]*prim.p[k][0])/p[j][j];
     else
      temp.p[j][0] -= (p[j][k]*temp.p[k][0])/p[j][j];} //for Gause-Seidel
   entry diff=abs(temp.p[j][0]-prim.p[j][0]);
   if(maxim<diff)maxim=diff;
  }
  prim=temp;
  if(maxim<eps)break;
 }
}


template <class entry>
void matrix<entry>::LU_factorize(matrix<entry> &L, matrix<entry> &U)const
{
 if(r!=c){show_error(11,0);return;}
 matrix<entry> u(*this),l(r,r);
 for(unsigned i=0;i<r;i++)
 {
  if(u.p[i][i]==entry(0)){show_error(12,0);return;}
  for(unsigned j=i+1;j<r;j++)
  {l.p[j][i]=u.p[j][i]*u.p[i][i];
   u.bro(j+1,i+1,l.p[j][i]);
   l.p[i][j]=entry(0);}

  l.p[i][i]=entry(1);}

 L=l;
 U=u;
}

template <class entry>
void matrix<entry>::show_error(unsigned i,bool termin)
{
 text_mode();
 gotoxy(1,23);
 cout<<"Matrix: ";
 switch(i)
 {
  case 0:cout<<"Try again...";return;
  case 1:cout<<"Memory allocation error.";break;
  case 2:cout<<"Out of bound.";break;
  case 3:cout<<"Matrixes are not multiplicable.";break;
  case 4:cout<<"Matrix is singular.";break;
  case 5:cout<<"Operation between non-equal dimension matrixes.";break;
  case 6:cout<<"Invalid row operation";break;
  case 7:cout<<"Invalid matrix. '[' or ']' are not found.";break;
  case 8:cout<<"Invalid matrix. Rows do not have equal number of enteries.";break;
  case 9:cout<<"Matrix for a linear equations system must be from dimension nx(n+1).";break;
  case 10:cout<<"Invalid sub-matrix requested.";break;
  case 11:cout<<"Matrix is not square.";break;
  case 12:cout<<"LU factorization is not possible, some row permutions are needed.";break;
  case 13:cout<<"Enter an nx1 vector for this method.";break;
 }
 getch();
 if(termin){gotoxy(1,25);cout<<"Abnormal termination by matrix object.";getch();abort();}
}

template <class entry>
void matrix<entry>::screen_display()
{
 string *s;
 s=new string[r];
 if(!s)show_error(1);

 text_info ti;
 gettextinfo(&ti);
 int x=1,y=2,spc=5,
     xc=x,yc=1,lc=1,
     w=ti.screenwidth, h=ti.screenheight;
 bool labeling=false;
 clrscr();

 if(r>h-1)
  for(unsigned i=0;i<c;i++)
  {cout<<char(9)<<"Column "<<(i+1)<<endl;
   for(unsigned j=0;j<r;j++)
   {cout<<"Row "<<(j+1)<<':'<<char(9);
    cout<<p[j][i]<<endl;
    y=wherey();
    if(y==h){cout<<"Press a key...";getch();clrscr();y=1;}
    gotoxy(1,y);}}

 else
 {
  for(unsigned i=0;i<c;i++)
  {unsigned max_column_width=1;

   for(unsigned j=0;j<r;j++)
   {convert_numeric_to_string(p[j][i],s[j]);
    if(s[j].length()>max_column_width)
    max_column_width=s[j].length();}

    if((max_column_width+x>w)||(labeling&&i==c-1))
    {labeling=true;
     gotoxy(xc,yc);
     if(!(max_column_width+x>w)&&(i==c-1))i=c;
     if(lc<i)cout<<"Columns "<<lc<<" to "<<i;

     lc=i+1;
     if(max_column_width+x>w)
     {if(y+2*r+2<h){x=1;yc=y+r+1;y=yc+1;xc=1;getch();}
      else{gotoxy(1,h);cout<<"Press a key...";getch();clrscr();y=2;x=1;yc=1,xc=1;}

      if((i==c-1))
      {gotoxy(xc,yc);
       cout<<"Column "<<c;}}
    }

    for(unsigned j=0;j<r;j++)
    {gotoxy(x,y);
     cout<<s[j];
     y++;}
    x+=max_column_width+spc;y-=r;
   }
  }
}


template <class entry>
ostream &operator<<(ostream &out,const matrix<entry> &a)
{
 for(counter i=0;i<a.r;i++)
 {for(counter j=0;j<a.c;j++)
  {out<<a.p[i][j];
   if(j!=a.c-1)out<<char(9);}
  if(i!=a.r-1)out<<'\n';}
 return out;
}

template <class entry>
istream &operator>>(istream &in,matrix<entry> &a)
{
 string s;
 if(in==cin)
 {using namespace Amin_math;
  get_from_IBM_keyboard(s);}
 else
 {in.unsetf(ios::skipws);
  char x;
  while(!in.eof()){in>>x;s+=x;}
 }
 a=matrix<entry>(s);
 return in;
}