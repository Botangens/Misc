#include <iostream>
//#include <type_traits> // actually required but seems to be automatically
                         // included with iostream
using namespace std;

// similar to 'auto_breaks_privateness' example - this is outer class
// representing some sort of resource manager with hidden (at least it should
// be hidden) inner subclass that stands for resource
class C
{
private:
    //!! Again this is 'totally' private subclass. All the constructors and
    // operator= are private. The type is also private. Everything is
    // private (like in nightmare of communists).
    // TODO: In addition one should test this with the nameless subclass and
    // auto return type for getters with C++14.
    class SubC
    {
        friend class C; // simple access for resource manager 'C'
    private:
        SubC() : i{} // simple default constructor for monitoring the objects
        {
            cout << "SubC()" << endl;
        }
        SubC( const SubC& sc ) : i{ sc.i } // yet another simple constructor
                                           // for monitoring SubC objects
        {
            cout << "SubC1(SubC)" << endl;
        }
        SubC& operator= ( const SubC& sc ) // like above - just for monitoring
        {
            i = sc.i;
            cout << "SubC =" << endl;
        }

        // This one has a bit complicated consequences. I`ll explain it later.
        //~SubC() { cout << "~SubC()" << endl; }
    public:
        int i;
    } sub; // This is NOT static variable. If you want some - put 'static'
           // before 'class SubC'. That will influence this very object.

public:
    void setI( int _i ) { sub.i = _i; } // just some setter...
    int getI() { return sub.i; }        // ...and getter.

    // These are important! The difference is a qualifier of return value.
    SubC getS_c() { return sub; } // for 'get SubC copy'
    const SubC getS_cc() { return sub; } // for 'get SubC const copy'
    const SubC& getS_cr() { return sub; } // for 'get SubC const reference'
};

// --------------------------------------------------------------------------

// Easy printer for in-template-usage example
template<class C>
struct P { P( const C& c ) { cout << c.i << endl; } };

// ================================ The main ================================

int main()
{
    // creating some data to watch over
    C c;
    c.setI( 3 );

    cout << "\n------------- copy example --------------\n";

    cout << c.getI() << endl; // monitor current original .i value

    // Imagine that one needs to allow observing this resource and does not
    // want to provide any access nor even leave any trace of original
    // resource. Thus he make the resource type hidden and all the
    // constructors private. And... that does not help anyway)
    typedef decltype( c.getS_c() ) T1;
    auto& a1 = const_cast< T1& >( ( const T1& )c.getS_c() );
    // You may notice '&' after 'auto' keyword but don`t be confused - a1 is
    // still the copy, not a reference to hidden resource. So this is the way
    // to create new instances of inaccessible class.

    // The type T1 is just to make lines shorter, yet it is valid type which
    // could be used for example in templates:
    P<T1> p( a1 );

    //// This example shows C++ casts headache. The fact that some object is
    //// temporal may be ignored. Moreover Microsoft compiler allows even
    //// easier way to do this:
    //auto& a1 = c.getS_c();
    //// Considering current circumstances I`d say that it`s not even bad
    //// after all... (doesn`t deny the fact itself)

    a1.i = 42; // ordinary assignments as a1 is just a simple variable
    cout << c.getI() << "\t" << a1.i << endl; // both original and changed

    cout << "\n---------- const copy example -----------\n";

    cout << c.getI() << endl; // monitor current original .i value

    // One may think that those 'variable' manipulations could have been
    // prevented by adding a 'const' qualifier to return value of getter
    // function so that nobody could modify those variables and therefore
    // will not create them.
    // Wrong!
    // Now watch some 'big bad woodoo':
    auto& a2 = const_cast< decay< decltype( c.getS_cc() ) >::type& >
        ( ( const decltype( c.getS_cc() )& )c.getS_cc() );
    // Once more - Microsoft compiler would let this line be like:
    // auto& a2 = const_cast< decay< decltype( c.getS_cc() ) >::type& >( c.getS_cc() );
    // marking return value with 'const' automatically. By the way - the cast
    // TO const may also be performed by static_cast or dynamic_cast

    cout << a2.i << endl; // stolen value
    a2.i = 42; // same simple assignments as in a1 case

    //// AND this time there is another 'solution': as the return value is
    //// already 'const' - you may get the copy in simple way and only use
    //// that casts to change the value. Everything else is just the same
    //// as before:
    //auto& a2 = c.getS_cc();  //same
    //cout << a2.i << endl; // stolen value
    //const_cast< decay< decltype( a2 ) >::type& >( a2 ).i = 42;

    cout << c.getI() << "\t" << a2.i << endl; // both original and changed

    cout << "\n-------- const reference example --------\n";

    cout << c.getI() << endl; // monitor current original .i value

    // In some cases the 'hidden' resource may be too large or complicated
    // to make a copy for observing. Also one may consider that if he avoid
    // copying inside the getter - no instances could be created because of
    // private constructors. That will save the day, won`t it?
    auto& a3 = const_cast< decay< decltype( c.getS_cr() ) >::type& >( c.getS_cr() );
    // Well, that will actually forbid the copying as a3 is a reference, BUT!!

    // The result is much worse. The technique demonstrated above is still
    // applicable, and this time it will grant access to the inner data itself!
    cout << a3.i << endl; // stolen value
    a3.i = 42; // violative assignment
    cout << c.getI() << "\t" << a3.i << endl; // both original and violated

    cout << "\n----------\n";

    // Now some words about private destructor.
    // Uncommenting the line '//~SubC() { cout << "~SubC()" << endl; }'
    // as a public constructor won`t change anything. At the same time
    // making it private will make several important results.
    // By doing it one will forbid the destruction of object anywhere
    // outside the 'C' resource manager so no copy couldn`t be created.
    // But (there are always some 'but's) that will also forbid temporal
    // objects thus no one could observe the data by copy-getter function:
    // cout << c.getS_c().i << endl; // error: "~SubC()" is inaccessible.
    // cout << c.getS_cr().i << endl; // valid

    cin.get();
    return 0;
}

