#include <iostream>
#include "number.h"
#include "rational.h"

main()
{
    rational a,x;
    rational::mode=1;
    for(int i=0;i<1000;i++)
    {
        number b(i);
        a+=rational(number::One,b.fact());
    }
    for(int i=0;i<1000;i++)
    {
        number b(i);
        x+=rational(number::One,b.fact());
        cout<<i<<">> x size:"<<x.size()<<"- a size:"<<a.size()<<" a-x="<<(a-x)<<"\n";
    }
    rational result;
    result.set(string("2+61170094524238231939040148079/85161690162019752273248256000"));
    cout<<"FINAL>>"<<">> result size:"<<result.size()<<"- x size:"<<a.size()<<" a-x="<<(a-x)<<"\n";

}
