#include <iostream>
#include <type_traits>
#include <typeinfo>
using namespace std;

// This example demonstrates C++ capabilities of callable objects.
// Of course it does not contain all possible cases but generally should be
// useful to show and describe some basic yet non-obvious features.
//
// The comments may sometimes be hard to read - the example is fitted for
// easier code reading, not the commentaries.

// =========================== Testing utils ================================

// Pay attention: these macros are the very first kind of function-like
// instances. Yet they are not really callable nor even objects...

// Shortcut for std::is_same printing
#define Same( X, Y ) \
  cout << #X << "\t = " << #Y << "\t" << std::is_same<X, Y>() << endl;

// Shortcut for cute typeid printing
#define Tprint( X ) \
  cout << #X << "\t: " << typeid( X ).name() << endl;

// --------------------------------------------------------------------------

// 'Direct call test' - simply a call of an object (any kind) and cout<< the
// result of that call.
#define D_test( X )  cout << X() << endl;

// 'Template test' - a function with templated argument which should be
// 'callable object' (aka function or functor). The argument is being called
// inside the function, than the result of the call is printed.
template<class C>
void T_test( C c ) { cout << c() << endl; }

// 'Function test' F_test - the function with the argument of the exact type -
// int (*)() - a function with no arguments which returns int.
// It is being called inside F_test and than the result of the call is printed.
void F_test( int( *f )() ) { cout << f() << endl; }

// ============================= The MAIN ===================================

int main()
{
    cout << std::boolalpha;  // for nice bool printing

    int     foo();  // 1   // a function returning 1, defined after main.
    double dfoo();  // 2.2 // some other function returning 2.2, defined after main.
    auto ai = []()->int { return 3; };      // lambda expression stored in auto. Returns 3.
    auto ai1 = []()->int { return 3; };     // other lambda - body and return are the same, but the type is different!
    auto ad = []()->double { return 4.4; }; // lambda with another return type.
    union { inline int operator()() { return 5; } } ufoo;   // in this case union behaves just like a struct (it may have methods, constructors, destructor...)
    struct { inline int operator()() { return 6; } } ofoo;  // nameless struct object with operator(), thus it`s a functor.
    struct S { static inline int foo() { return 7; } };     // static member function. "What does it change?" one may ask.
    struct Cfoo { inline operator int  () { return 8; } };  // some struct without operator(), yet you may call the constructor - it always exists!

    // Just some types declarations for further processing
    typedef int ( *TF )();          // C-style function pointer
    using UF = int ( *)();          // C++ function pointer
    using UDF = decltype(foo)*;     // being declared in this way requires '*' to be the pointer! Yes - you assign object of type 'func' to the object of type 'func*'!
    using UDA = decltype(ai);       // lambda is an object, thus UDA does not have to be a pointer for assignment operations (in opposite to functions case)
    using UDA1 = decltype(ai1);     // other lambda = other type, same with structs/unions
    using UDU = decltype(ufoo);     // a type for nameless object... What a wonderful language)
    using UDO = decltype(ofoo);     // the same as above
    using UDS = decltype(S::foo)*;  // a type for static member function pointer
    // Cfoo                         // this is already the name of the type!

    cout << "------------------------ Type names ----------------------------\n";

    Tprint( TF );
    Tprint( UF );
    Tprint( UDF );
    Tprint( UDA );
    Tprint( UDA1 );
    Tprint( UDU );
    Tprint( UDO );
    Tprint( UDS );
    Tprint( Cfoo );

    cout << "----------------------- Type equality --------------------------\n";

    Same( UF, UDF );  // these should be the same. The difference is only a matter of taste.
    Same( UF, UDA );  // function pointer and lambda are NOT the same. But! You may be surprised a bit later...
    Same( UF, UDO );  // of course these are different. Just in case...
    Same( UDA, UDO ); // -//-
    Same( UF, UDS );  // static member function has the same type as simple function!

    cout << "------------------------ Direct call ---------------------------\n";

    // all direct calls are valid. For these objects you may always write 'int i = Obj();' or something alike.
    D_test( foo );
    D_test( dfoo );
    D_test( ai );
    D_test( ad );
    D_test( ufoo );
    D_test( ofoo );
    D_test( S::foo );
    D_test( Cfoo );

    cout << "------------------- Template argument call ---------------------\n";

    T_test( foo );
    T_test( dfoo );
    T_test( ai );
    T_test( ad );
    T_test( ufoo );
    T_test( ofoo );
    T_test( S::foo );
//  T_test( Cfoo );  // this one can`t be passed to template: it is not a callable object. It`s a TYPE!

    cout << "------------------- Function argument call ---------------------\n";

    F_test( foo );       // this is the only one suitable object. Oh, wait, it`s not!
//  F_test( dfoo );
    F_test( ai );        // !lambdas may be reduced to functions, but only with empty capture.
//  F_test( ad );
//  F_test( ufoo );
//  F_test( ofoo );
    F_test( S::foo );    //  actually static member functions are simply scoped functions...
//  F_test( Cfoo );

    cout << "------------------- Function pointers call ---------------------\n";

    // As you may notice - lambdas are reducible. Thus we should check general reduction possibilities.

    UF pfoo1 = foo;     // +function may be assigned to function pointer,
//  UF pfoo2 = dfoo;    // -but their types must match.
    UF pfoo3 = S::foo;  // +scoped (static member) functions still match the type.
    UF pfoo4 = ai;      // +lambda may be assigned to function pointer,
    UF pfoo5 = ai1;     // +any lambda with matching type.
//  UF pfoo6 = ofoo;    // -no other objects may be assigned to function pointer.

//  UDA pa1 = foo;      // -reversed order (lambda = func) assignment s forbidden!
//  UDA pa2 = ad;       // -no conversion between different lambdas.
    UDA pa3 = ai;       // +only the very lambda that has been used to declare the type.
//  UDA pa4 = ai1;      // -not even if the body is equal.
//  UDA pa5 = ofoo;     // -and of course no other objects.

    // after that - each assigned pointer/object can be called
    F_test( pfoo1 );
    F_test( pfoo3 );
    F_test( pfoo4 );
    F_test( pfoo5 );
    F_test( pa3 );

    // Look through the output!

    cout << "\n-----------------------------------------------------------------\n";
    cin.get();
    return 0;
}

// ========================= Functions` definitions =========================

int foo() { return 1; }
double dfoo() { return 2.2; }
