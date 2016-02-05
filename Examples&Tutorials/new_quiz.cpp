#include <iostream>
#include <typeinfo>
using namespace std;

// simply for nice printing
template<class X>
void t_print(const X& x )
{
  cout<< *(int*)(& x )<<"\t"<<typeid( x ).name()<<endl;
}

int main()
{

  int i0, i1 {}, i2 = {}, i3(), i4 = int();

  auto ai2 = i2;
  auto ai3 = i3;
  auto ai4 = i4;

  t_print( i0 );
  t_print( i1 );
  t_print( i2 );
  t_print( i3 );
  t_print( i4 );

  cout<<endl;

  t_print( ai2 );
  t_print( ai3 );
  t_print( ai4 );

  // predict the output!!
  cin.get();
  return 0;
}

int i0() { return {}; }
int i1() { return {}; }
int i2() { return {}; }
int i3() { return {}; }
int i4() { return {}; }