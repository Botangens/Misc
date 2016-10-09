/*
 *  Namespace 'ConstexprNmspc
 *
 *  Exotic constexpr counter for generic purposes. Can be used inside
 *  another template. 
 *
 *  WARNING!: only tested with g++, vc++ and clang for c++11/c++14.
 */

// ==========================================================================

namespace ConstexprNmspc
{

    // Tagging type: 'int' interprite parameter as a type (more abstract).
    template<int N = 0> struct TagType { constexpr static int v = N; };

// =========================== Initial Values ===============================

    // Flag-function declarator
    template<class T>
    struct Declar
    {
        friend constexpr int _initVal( Declar<T> );
        friend constexpr int _stepVal( Declar<T> );
    };

    // Flag-function definer
#if defined(__clang__)

    template<int Init, int Step, class T>
    struct CounterInits
    {
        friend constexpr int _initVal( Declar<T> ) { return Init; }
        friend constexpr int _stepVal( Declar<T> ) { return Step; }
    };

#elif defined(__GNUG__) || defined(__GNUC__) || defined(_MSC_VER)

    template<int Init, int Step, class T, bool B =  // 'B' for static assert
        noexcept(_initVal( Declar<T>() ) + _stepVal( Declar<T>() )) >
    struct CounterInits
    {
        static_assert(!B, "!!! Counter is already initialized !!!\n");

        friend constexpr int _initVal( Declar<T> ) { return Init; }
        friend constexpr int _stepVal( Declar<T> ) { return Step; }
    };

#else
    static_assert(false, "This compiler is not tested for compatibility!");
#endif

    // Initialize counter parameters
    template<int I, int S = 1, class T = TagType<>, int R = sizeof( CounterInits<I, S, T> )>
    constexpr int init() { return R; }

    // Parameter overload for int 'ID'
    template<int I, int S, int ID, int R = sizeof( CounterInits<I, S, TagType<ID>> )>
    constexpr int init() { return R; }

// ------------------------------ Getters -----------------------------------

#if defined(__clang__)

    template<class T, int R = _initVal( Declar<T>() )>
    constexpr int _getInit( int ) { return R; }

    template<class T>
    constexpr int _getInit( float ) { return 0; }

    // Get counter initial value
    template<class T = TagType<>, int R = _getInit<T>(0) >
    constexpr int getInit() { return R; }


    template<class T, int R = _stepVal( Declar<T>() )>
    constexpr int _getStep( int ) { return R; }

    template<class T>
    constexpr int _getStep( float ) { return 1; }

    // Get counter step
    template<class T = TagType<>, int R = _getStep<T>(0) >
    constexpr int getStep() { return R; }

#elif defined(__GNUG__) || defined(__GNUC__) || defined(_MSC_VER)

    // Get counter initial value 
    template<class T = TagType<>, int R = 
        noexcept( _initVal(Declar<T>()) ) ? _initVal(Declar<T>()) : 0 >
    constexpr int getInit() { return R; }
    
    // Get counter step
    template<class T = TagType<>, int R =
        noexcept( _stepVal(Declar<T>()) ) ? _stepVal(Declar<T>()) : 1 >
    constexpr int getStep() { return R; }

#else
    static_assert(false, "This compiler is not tested for compatibility!");
#endif

    // Parameter overload for int 'ID'
    template<int ID, int R = getInit<TagType<ID>>()>
    constexpr int getInit() { return R; }

    // Parameter overload for int 'ID'
    template<int ID, int R = getStep<TagType<ID>>()>
    constexpr int getStep() { return R; }

// =============================== Counter ==================================

    // Flag-function declarator
    template<class T, int N>
    struct Flag { friend constexpr int flag( Flag<T, N> ); };

    // Flag-function definer
    template<class T, int N>
    struct Setter { friend constexpr int flag( Flag<T, N> ) { return N; } };

// --------------------------- Counting functions ---------------------------

    template<class T, int N> // Setter in argument (not template parameter) for clang
    constexpr int _count( float, Flag<T, N>, int = sizeof( Setter<T, N> ) ) { return N; }

    template<class T, int N, int = flag(Flag<T, N>()), int R = _count(0, Flag<T, N + getStep<T>()>())>
    constexpr int _count( int, Flag<T, N> ) { return R; }

    // Get the value and increment counter
    template<class T = TagType<>, int N = getInit<T>(), int R = _count(0, Flag<T, N>())>
    constexpr int count() { return R; }

    // Parameter overload for int 'ID'
    template<int ID, int N = getInit<TagType<ID>>(), int R = _count(0, Flag<TagType<ID>, N>())>
    constexpr int count() { return R; }

// --------------------------- Observing functions --------------------------

    template<class T, int N>
    constexpr int _observe( float, Flag<T, N> ) { return N; }

    template<class T, int N, int = flag(Flag<T, N>()), int R = _observe(0, Flag<T, N + getStep<T>()>())>
    constexpr int _observe( int, Flag<T, N> ) { return R; }

    // Get current counter value
    template<class T = TagType<>, int N = getInit<T>(), int R = _observe(0, Flag<T, N>())>
    constexpr int observe() { return R; }

    // Parameter overload for int 'ID'
    template<int ID, int N = getInit<TagType<ID>>(), int R = _observe(0, Flag<TagType<ID>, N>())>
    constexpr int observe() { return R; }

} // namespace ConstexprNmspc

// ==========================================================================
