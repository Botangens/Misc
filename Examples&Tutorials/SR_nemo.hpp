#ifndef SR_NEMO_HPP_
#define SR_NEMO_HPP_

namespace Namespace
{

    class A
    {
        template <class O> friend class D;
        template <class O> friend class B;
    protected:
        size_t n{ 0 };
        virtual ~A() {};
    };


    template<class T = void*>
    class B : protected A
    {
        template <class O> friend class D;
    protected:
        T* ptr{ nullptr };
        bool property{ false };

        B() { n = 1; }
        B( const T& TC, size_t i ) : ptr( new T[i] ), property( true ) { while (i > 0) ptr[--i] = TC; }
        B( T* PT ) : ptr( PT ) {}
        B( T* PT, int ) : ptr( PT ), property( true ) {}
        ~B() { if (property) delete[] ptr; }
    };


    class C : B<> { template <class O> friend class D; } instance;




    template<class T>
    class D
    {
        template <class O> friend class D;
    protected:
        A* pa{ nullptr };

        inline void _method( const A* c )
        {
            method3();
            ++(pa = (A*)c)->n;
        }

    public:
        D() { ++(pa = &instance)->n; }
        D( T* PC ) { ++(pa = new B<T>( PC ))->n; }
        template <class T_T>
        D( T* PC, T_T ) { ++(pa = new B<T>( PC, 0 ))->n; }
        D( const T& TC, const size_t& i = 1 ) { ++(pa = new B<T>( TC, i ))->n; }
        D( const D<T>& DC ) { ++(pa = DC.pa)->n; }
        template <class O>
        D( const D<O>& DC ) { ++(pa = DC.pa)->n; }

        ~D() { method3(); }

        D& operator=( const C& CC )
        {
            _method( &CC );
            return *this;
        }
        D& operator=( const D& DC )
        {
            _method( DC.pa );
            return *this;
        }
        template <class O>
        D& operator=( const D<O>& DC )
        {
            _method( DC.pa );
            return *this;
        }

        inline T& operator *() { return *((B<T>*)pa)->ptr; }
        inline T*& operator ->() { return ((B<T>*)pa)->ptr; }
        inline operator T&() { return *((B<T>*)pa)->ptr; }
        inline operator T*&() { return ((B<T>*)pa)->ptr; }

        inline size_t method1() { return pa->n; }


        inline void method2() { if (pa) ++pa->n; }
        inline void method3() { if (pa && !(--pa->n)) delete pa; }
    };

    template <class T>
    inline D<T> Func( const size_t& i, const T& TC = T() ) { return D<T>( TC, i ); }
    template <class T>
    inline D<T> Func( T* pc ) { return D<T>( pc, true ); }
};

#endif