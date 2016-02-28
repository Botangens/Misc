#include <iostream>
using namespace std;

struct A
{
    virtual void operator()() { cout << "A" << endl; }
};

struct B : public A
{
    void operator()() { cout << "B" << endl; }
};

int main()
{

    B b;
    A   a = b,
        &ra = b,
        *pa = &b,
        ria = reinterpret_cast<A&>(b),
        &rira = reinterpret_cast<A&>(b),
        *ripa = reinterpret_cast<A*>(&b);

    // predict the output!
    b();
    a();
    ra();
    (*pa)();
    ria();
    rira();
    (*ripa)();


    cin.get();
}
