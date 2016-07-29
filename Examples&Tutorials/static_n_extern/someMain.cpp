/*
 * Small example of 'static' and 'extern' keywords.
 */

#include <iostream>
using namespace std;

#include "someHead.hpp"

#define TEST( X )         \
  set##X(3);              \
  cout<<get##X()<<endl;   \
  blah##X();              \
  cout<<get##X()<<endl;

int main ()
{

  TEST(1)
  cout<<_get1()<<endl;
  cout<<"---------------------------\n";

  TEST(2)
  cout<<_get2()<<endl;
  cout<<"---------------------------\n";

  TEST(3)
//  cout<<_get3()<<endl; // no such function
  cout<<"---------------------------\n";

  TEST(4)
//  cout<<_get4()<<endl; // no such function
  cout<<"---------------------------\n";

  TEST(5)
  cout<<_get5()<<endl;
  cout<<"---------------------------\n";

  return 0;
}
