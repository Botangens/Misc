#include <iostream>
#include <typeinfo>
using namespace std;

// simply for nice printing
#define T_PRINT( X ) \
{ \
  cout<< #X << ":\t" << *(int*)(& X ) << "\t" \
    << &X << "\t" << typeid( X ).name() << endl; \
}

int main()
{
  struct { int operator()(){return {};} } i;

  int i0, i1 {}, i2 = {}, i3(), i4 = int(), i5 = i(), i6 = i3();

  auto ai2 = i2;
  auto ai3 = i3;
  auto ai4 = i4;

  cout << "name\tval\taddress\t\ttypename" << endl << endl;

  T_PRINT( i0 );
  T_PRINT( i1 );
  T_PRINT( i2 );
  T_PRINT( i3 );
  T_PRINT( i4 );
  T_PRINT( i5 );
  T_PRINT( i6 );
  T_PRINT( i );

  cout<<endl;

  T_PRINT( ai2 );
  T_PRINT( ai3 );
  T_PRINT( ai4 );

  // predict the output!!
  cin.get();
  return 0;
}

int i0() { return {10}; }
int i1() { return {11}; }
int i2() { return {12}; }
int i3() { return {13}; }
int i4() { return {14}; }
int i5() { return {15}; }
int i6() { return {16}; }
int i() { return {101}; }
