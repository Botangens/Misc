// NOTE: this example is valid for MSVS and respective 
// compiler. Result may be different with other compilers.

#include <iostream>
using namespace std;

class C
{
private: 
    //!! private hidden class with private constructor. We should not be able
    // to create an instance of SubC anywhere out of C, but...
    class SubC
    {
        friend class C; //without this C() constructor will be deleted
    private:
        SubC() = default; //private constructor to prevent creation of objects of this type
        //SubC( const SubC& ) = default; // actually this private constructor could have saved the day, 
                                         // but this example is to show what`s happenning without it.

    public:
        int i;
    } sub; // note - this is not static

public:
    void setSubI( int _i ) { sub.i = _i; } // simple setter
    const SubC& getSub() { return sub; } // const reference - observer fuction,
                                         // made for calls like 'c.getSub().i;'
};

int main()
{
    //setting inner variables
    C c1, c2;
    c1.setSubI( 3 );
    c2.setSubI( 5 );

    // WATCH CLOSELY! now we are creating the the instance of totally private
    // class.
    auto a = c1.getSub();
    auto& ra = c2.getSub();

    cout << a.i << "\t" << ra.i << endl;
    //ra.i = 9;  // this is forbidden coz of 'const' in return type
    a.i = 7;     // but this is OK
    
    cout << c1.getSub().i << "\t" << a.i << endl;
    
    // Do not worry - the type itself remains (allmost) inaccessible so 
    // one can`t inherite it nor use in a template - only create auto 
    // object. In addition making thi copy constructor private WILL 
    // prevent from this 'typeleak', however one can always create 
    // a reference like: 'auto& a = getSub();'
    cin.get();
}
