/*
 * This is example for template styling technique - parameters encapsulation.
 * Inspired by Javascript workaround for named arguments - a simple, clear
 * yet flexible trick was developed and is shown in this file.
 *
 * The idea is to hide all optional parameters into some structure and pass
 * this structure as a single defaulted type-parameter of target template.
 * This is somelike close to Allocator abstraction.
 *
 * In the 'classic' approach target template should have multiple (sometimes
 * a lot of) parameters, those should be specified independently. This makes it
 * hard to specify few latter parameters omitting the former ones.
 *
 * The example demonstrate the cases of different parameters specifications
 * for 7-parameter template, those are implemented in 'classic' and
 * 'encapsulated' styles.
 *
 * NOTE: clang compiler (linker indeed) has some sort of bug with 
 * 'const char*' type member. In this very example Linker reports about 
 * undefined reference to 'MyRegistry2<MySets>::descStr'.
 */

#include <typeinfo>
#include <iostream>

#include <string>
#include <unordered_map>

using namespace std;

// ==========================================================================

// Classic implementation - single template class with 7 parameters with
// default values. According to C++ rules - floating point value may not be
// a template parameter, thus it should be passed (for example) as a ratio.
#include <ratio>

// Kinda custom registry class with a lot of parameters for best performance.
template<class ValueType            = std::string,
         class KeyType              = int,
         class MapType              = std::unordered_map<KeyType, ValueType>,
         int initialSize            = 8,
         int flatExtension          = 8,
         class extensionFactor      = std::ratio<3, 2>,
         const char* descriptionStr = nullptr>
class MyRegistry1
{
public:
    using Value_t = ValueType;
    using Key_t = KeyType;
    using Map_t = MapType;
    static const int initSize = initialSize;
    static const int flatExt = flatExtension;

    constexpr static double extFac =                // double (non-integral)
        static_cast<float>(extensionFactor::num) /  // member should be passed
        static_cast<float>(extensionFactor::den);   // as integrals and
                                                    // calculated afterwards
    static const char* descStr;

    Map_t map;
    Value_t someValue;
    //...
};

// This is a must for proper const char* member initialization. Otherwise
// 'constexpr' modifier is required, which causes bugs in some compilers.
template<class c1, class c2, class c3, int i1,
    int i2, class c4, const char* ds>
const char* MyRegistry1<c1, c2, c3, i1, i2, c4, ds>::descStr = ds;

// --------------------------------------------------------------------------

// Encapsulated implementation - possibly templated structure (class, union,
// whatever) stores a set of parameters. Single-parameter template class with
// that structure as the default value. No extra rules, except 'constexpr' for
// in-class-initializations.

// This may be non-template type. It is templated just to demonstrate the
// flexibility of technique. Changing of optional parameters may be performed
// by simply overloading (shadowing) them in inherited structure, that should
// afterwards be passed as a custom parameters set.
template<class V = std::string, class K = int>
struct DefaultSettings
{
    using Value_t = V;
    using Key_t = K;
    using Map_t = std::unordered_map<Key_t, Value_t>;
    static const int initSize = 8;
    static const int flatExt = 8;
    constexpr static double extFac = 1.5;
    constexpr static const char* descStr = nullptr;
};

// If required - several non-optional parameters (like value type) may always
// be present.
template<class Settings = DefaultSettings<>>
class MyRegistry2
{
public:
    using Value_t = typename Settings::Value_t;   // 'typename' keyword is
    using Key_t = typename Settings::Key_t;       // required for type-member
    using Map_t = typename Settings::Map_t;       // accessing
    static const int initSize = Settings::initSize;
    static const int flatExt = Settings::flatExt;
    constexpr static double extFac = Settings::extFac;  // double is that easy
    constexpr static const char* descStr = Settings::descStr;

    Map_t map;
    Value_t someValue;
    //...
};

// ==========================================================================
// The 'main' functions for both cases are demonstrated here. Real main just
// calls them consequently.
// ==========================================================================

// Some pretty-print macros
#define TPrint( X ) cout << typeid( X ).name() << endl; 
#define SPrint( X ) if( X ) cout << X << endl; 


// This is necessary for passing string as a template parameter.
// It should be placed in global scope.
// NOTE: array semantics ('[]' instead of '*') is a must!
char DESC[] = "just some text";

int main1()
{
    // Simplest case: everything by default.
    MyRegistry1<> s1;
    TPrint( s1.someValue );

    // Mid complexity: only 'important' front parameters are passed
    MyRegistry1<double, size_t> m1;
    TPrint( m1.someValue );

    // The hard one: in order to set the last parameters one have to 
    // additionally pass everything before them. Normal default parameters
    // may somehow be 'extracted' from the template, yet if some parameter is
    // being stored in different way than it was pass 
    // (for example ratio->double) it is impossible to require it.
    using MR1 = MyRegistry1<>;    // just to make it short
    MyRegistry1<
        MR1::Value_t, MR1::Key_t, MR1::Map_t, MR1::initSize, MR1::flatExt,
        std::ratio<3, 2>,  //<-must be passed manually, despite it is optional
        DESC    // <-- here it is - everything before must by passed just to
        > h1;   // reach this point!
    SPrint( h1.descStr );

    return 0;
}

// -------------------------------------------------------------------------

// This is how required parameters may be changed by name. Everything else
// remains at default values. 
// NOTE: any kind of values (even non-integral and member functions)
// may be specified as they are (without any tricks like ratio for double).
// The only thing required is: any non-integral parameter should be marked 
// with constexpr and thus such settings structure should be defined at 
// global scope. Otherwise it may be local type.
struct MySets : public DefaultSettings<>
{
    constexpr static double extFac = 2.5;
    constexpr static const char* descStr = "just some text";
};

int main2()
{
    // Simplest case: does not change at all
    MyRegistry2<> s2;
    TPrint( s2.someValue );

    // Mid complexity: front parameters may be passed as 'Settings' type
    // parameters or in traditional way if that is provided by the template
    MyRegistry2<DefaultSettings<double, size_t>> m2;
    TPrint( m2.someValue );

    // Hard one: is not that hard after all. One should just pass the custom
    // settings type
    MyRegistry2<MySets> h2;
    SPrint( h2.descStr );

    return 0;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// The output and thus the resulting effect are the same.
int main ()
{
    main1();

    cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n";

    main2();

    cin.get();
    return 0;
}

// ==========================================================================
