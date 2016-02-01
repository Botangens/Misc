#include <iostream>
using namespace std;

//  nonvirtual inheritance
struct A1
{
    int i;
    ~A1() {}
};

struct B1 : A1 { ~B1() {}; };

// virtual inheritance
struct A2
{
    int i;
    virtual ~A2() {}
};

struct B2 : A2 { virtual ~B2() {}; };

// ---------- the main ------------
// question: what will be the output?
int main()
{
    cout << "\nnonvirtual:\n";

    A1 a1;
    B1 b1;
    A1* paa1 = &a1, *pab1 = &b1;
    B1* pb1 = &b1;

    cout << typeid(*paa1).name() << endl;
    cout << typeid(*pab1).name() << endl;
    cout << typeid(*pb1).name() << endl;

    cout << "\nvirtual:\n";

    A2 a2;
    B2 b2;
    A2* paa2 = &a2, *pab2 = &b2;
    B2* pb2 = &b2;

    cout << typeid(*paa2).name() << endl;
    cout << typeid(*pab2).name() << endl;
    cout << typeid(*pb2).name() << endl;

    cin.get();
    return 0;
}