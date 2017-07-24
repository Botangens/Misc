#ifndef SMARTREF_HPP_
#define SMARTREF_HPP_

namespace SmartReferenceCounter
{
    struct Counter //! abstract base class for counting references
    {
        template <class O> friend class SmartRef; //!< friend works with 'count' member
        template<class C = void*> class RefCounter; //!< data-handler scoped declaration

        size_t Count() const { return count; }; //!< number of references getter
        static auto& nullRef(); //!< access to the 'ground' counter
    protected:
        size_t count{ 0 }; //!< the counting variable
        virtual ~Counter() = 0; //!< this class is abstract coz can`t handle types
    };
    inline Counter::~Counter() = default; //!< required by 'abstract through destructor' mechanism

    template<class C>
    struct Counter::RefCounter : public Counter //! data-handling counter
    {
        template <class O> friend class SmartRef; //!< friend calls constructors
    protected:
        using StoredType = C; //!< stored type alias
        C* objP{ nullptr }; //!< The data
        bool deletable{ false }; //!< auxiliary info

        RefCounter() { count = 1; } //!< just a default constructor
        RefCounter( const C& cc, size_t s ) : objP( new C[s] ), deletable( true ) { while( s > 0 ) objP[--s] = cc; }  //!< constructing from entity duplicates value
        RefCounter( C* pc ) : objP( pc ) {}  //!< construction from pointer works with original
        RefCounter( C* pc, int ) : objP( pc ), deletable( true ) {} //!< array memory handling
        ~RefCounter() { if( deletable ) delete[] objP; }  //!< smart deletion
    };

    auto& Counter::nullRef() // singleton 'ground'-counter access
    {
        static struct : public Counter::RefCounter<> {} null {};
        return static_cast<Counter&>(null);
    }

    static const Counter& nullRef{ Counter::nullRef() }; //!< global 'null' reference (shortcut)

    //! \class SmartRef - Provides reference counting and memory handling.
    //! Behaves both like pointer to array and a bit like entity.
    template<class C>
    class SmartRef
    {
        template <class O> friend class SmartRef;  // Any SmartRef is a friend to any other SmartRef
        template <class O> using RefCounter = Counter::RefCounter<O>;
        C*& Obj() const { return ((RefCounter<C>*)refCnt)->objP; }  // No dynamic_cast - user MUST handle types!
    protected:
        mutable Counter* refCnt{ nullptr };  //!< actually the only data in SmartRef

        inline SmartRef& Copy( Counter* c )  //!< copy the counter (given as const coz calling methods take const refs)
        {
            decRef();
            ++(refCnt = c)->count;  // processing the counter
            return *this;
        }
    public:
        using ObjectType = C;  //!< template parameter alias

        SmartRef() { ++(refCnt = &Counter::nullRef())->count; }  //!< default constructor. Uses no object. Counts as nullRef
        explicit SmartRef( C* pc ) { ++(refCnt = new RefCounter<C>( pc ))->count; } //!< pointer constructor. Just counts further refs
        template <class Flag>
        explicit SmartRef( C* pc, Flag ) { ++(refCnt = new RefCounter<C>( pc, 0 ))->count; } //!< pointer-to-array constructor. Steals ref and handles memory
        SmartRef( const C& c, const size_t& s = 1 ) { ++(refCnt = new RefCounter<C>( c, s ))->count; } //!< array constructor with initialization. Total memory handling
        SmartRef( Counter& c ) { ++(refCnt = &c)->count; } //!< construction from counter. Allows tricks with nullRef
        SmartRef( const SmartRef<C>& sm ) { ++(refCnt = sm.refCnt)->count; } //!< same-type copy constructor. Provides proper ref counting and memory handling
        template <class Other>
        SmartRef( const SmartRef<Other>& sm ) { ++(refCnt = sm.refCnt)->count; } //!< cross-type copy constructor. Provides proper ref counting and memory handling

        ~SmartRef() { decRef(); }  //!< simple destructor

        SmartRef& operator=( const SmartRef& sm ) { return Copy( sm.refCnt ); } //!< same-type assignment operator. Supports copy constructor. MUST BE!
        template <class Other>
        SmartRef& operator=( const SmartRef<Other>& sm ) { return Copy( sm.refCnt ); } //!< cross-type assignment operator. Supports copy constructor

        C& operator *()   const { return *Obj(); }  //!< casting to object
        C*& operator ->() const { return Obj(); }  //!< casting to object`s props
        explicit operator C&() const { return *Obj(); }  //!< conversion to reference
        operator C*&() const { return Obj(); }  //!< conversion to pointer reference

        size_t Count() const { return refCnt->count; } //!< number of references getter

        // Dangerous low-level methods! Could be used for low-level control if needed
        inline void incRef() { if( refCnt ) ++refCnt->count; } //!< increasing reference counter
        inline void decRef() { if( refCnt && !(--refCnt->count) ) delete refCnt, refCnt = nullptr; }  //!< decreasing reference counter with calling of deletion
    };

    template <class C>  //!< Create self-clearing SmartRef which points to a new array of length 's' filled with copy of optional second argument
    inline SmartRef<C> SmartArray( const size_t& s, const C& TC = {} ) { return SmartRef<C>( TC, s ); }
    template <class C>  //!< Create self-clearing SmartRef which 'steals' the given reference
    inline SmartRef<C> SmartArray( C* pc ) { return SmartRef<C>( pc, true ); }
    template <class C>  //!< Create smart reference from value (for using with 'auto')
    inline SmartRef<C> Smartify( const C& c ) { return SmartRef<C>( c ); }

};  // \ namespace SmartReferenceCounter

#endif /* SMARTREF_HPP_ */
