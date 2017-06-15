#include <iostream>
using namespace std;

// Just an esoteric program - you may try to comprehend what is going on here.

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ CASE 0 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

struct S
{
  S& s;
  int v{-1};
  
  S(S& s): s(s) {}
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ CASE 1 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

struct Outer1
{
  struct Inner1;

  Inner1& i1;
};

struct Outer1::Inner1
{
  Outer1 o1;
  static const int v{-1};
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ CASE 2 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

struct Outer2
{
  struct Inner2;

  Inner2& i2;
  const int& v;
};

struct Outer2::Inner2
{
  Outer2 o2;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ CASE 3 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

struct Outer3
{
  struct Inner3;

  Inner3& i3;
  const int& v;

  Outer3(Inner3& r);
};

struct Outer3::Inner3
{
  friend struct Outer3;
private:
  static const int v;

public:
  Outer3 o3;

  Inner3(Outer3&& r): o3(r) {}
};

const int Outer3::Inner3::v {-3};

Outer3::Outer3(Inner3& r): i3(r), v(r.v){}

// ============================== MAIN ======================================

int main()
{
  // ------------- 0 ------------------
  
  S s{s};
  
  cout<<s.s.s.s.s.s.s.s.s.s.v<<endl;
  
  // ------------- 1 ------------------

  Outer1::Inner1 i1{i1};

  cout<<i1.o1.i1.o1.i1.o1.i1.v<<endl;

  // ------------- 2 ------------------

  Outer2::Inner2 i2{{i2, -2}};

  cout<<i2.o2.i2.o2.i2.o2.v<<endl;

  // ------------- 3 ------------------

  Outer3::Inner3 i3{{i3}};

  cout<<i3.o3.i3.o3.i3.o3.v<<endl;

  cout<<"_____________________"<<endl;
}
