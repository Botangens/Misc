#ifndef SMARTREF_HPP_
#define SMARTREF_HPP_

namespace SmartReferenceCounter
{
    class Holder //! namespace-like class to hide other classes in private
    {
      template <class O> friend class SmartRef;
      class Counter;
      class NullReference;
      template<class C = void*> class RefCounter;
    };

    struct Holder::Counter //! Utility virtual base class to handle derived templates
    {
        size_t count{ 0 };  //!< reference counter
        virtual ~Counter() {};  //!< virtual destructor - that is the purpose of this class existance
    };

    template<class C>
    class Holder::RefCounter : public Counter //! Utility data handler. Provides safe deletion.
    {
        template <class O> friend class SmartRef;
    protected:
        using StoredType = C;  //!< template parameter alias
        C* objP{ nullptr };  //!< object handle itself
        bool deletable{ false };  //!< signifies if the pointer was 'stolen'

        RefCounter() { count = 1; }  //!< default does not use object
        RefCounter(const C& cc, size_t s) : objP(new C[s]), deletable(true) { while (s > 0) objP[--s] = cc; }  //!< constructing from entity duplicates value
        RefCounter(C* pc) : objP(pc) {}  //!< construction from pointer works with original
        RefCounter(C* pc, int) : objP(pc), deletable(true) {} //!< array memory handling
        ~RefCounter() { if (deletable) delete[] objP; }  //!< smart deletion
    };

    // Cunning class for nullifying references (like nullptr) in default constructors.
    static class Holder::NullReference final : public Holder::RefCounter<> { size_t Count() const { return count; }; } nullRef;

    //! \class SmartRef - Provides reference counting and memory handling.
    //! Behaves both like pointer to array and a bit like entity.
    template<class C>
    class SmartRef
    {
        using Counter = Holder::Counter;
        using NullReference = Holder::NullReference;
        template<class O> using RefCounter = Holder::RefCounter<O>;
        C*& Obj() const { return ((RefCounter<C>*)refCnt)->objP; }
    public:
        template <class O> friend class SmartRef;  // Any SmartRef is a friend to any other SmartRef
        using ObjectType = C;  //!< template parameter alias
    protected:
        Counter* refCnt{ nullptr };  //!< actually the only data in SmartRef

        inline SmartRef& Copy(const Counter* c)  //!< copy the counter (given as const coz calling methods take const refs)
        {
            decRef();
            ++(refCnt = (Counter*)c)->count;  // unconst the counter and processing
            return *this;
        }
    public:
        SmartRef() { ++(refCnt = &nullRef)->count; }  //!< default constructor. Uses no object. Counts as nullRef
        explicit SmartRef(C* pc) { ++(refCnt = new RefCounter<C>(pc))->count; } //!< pointer constructor. Just counts further refs
        template <class Flag>
        explicit SmartRef(C* pc, Flag) { ++(refCnt = new RefCounter<C>(pc, 0))->count; } //!< pointer-to-array constructor. Steals ref and handles memory
        SmartRef(const C& c, const size_t& s = 1) { ++(refCnt = new RefCounter<C>(c, s))->count; } //!< array constructor with initialization. Total memory handling
        template<class Other>
        SmartRef(RefCounter<Other>& rc) { ++(refCnt = &rc)->count; } //!< construction from counter. Allows tricks with nullRef
        SmartRef(const SmartRef<C>& sm) { ++(refCnt = sm.refCnt)->count; } //!< same-type copy constructor. Provides proper ref counting and memory handling
        template <class Other>
        SmartRef(const SmartRef<Other>& sm) { ++(refCnt = sm.refCnt)->count; } //!< cross-type copy constructor. Provides proper ref counting and memory handling

        ~SmartRef() { decRef(); }  //!< simple destructor

        SmartRef& operator=(const SmartRef& sm) { return Copy(sm.refCnt); } //!< same-type assignment operator. Supports copy constructor. MUST BE!
        template <class Other>
        SmartRef& operator=(const SmartRef<Other>& sm) { return Copy(sm.refCnt); } //!< cross-type assignment operator. Supports copy constructor

        C& operator *()   const { return *Obj(); }  //!< casting to object
        C*& operator ->() const { return Obj(); }  //!< casting to object`s props
        explicit operator C&() const { return *Obj(); }  //!< conversion to reference
        operator C*&() const { return Obj(); }  //!< conversion to pointer reference

        size_t getCount() const { return refCnt->count; }

        // Dangerous methods! Could be used for low-level control in needed
        inline void incRef() { if (refCnt) ++refCnt->count; } //!< increasing reference counter
        inline void decRef() { if (refCnt && !(--refCnt->count)) delete refCnt, refCnt = nullptr; }  //!< decreasing reference counter with calling of deletion
    };

    template <class C>  //!< Create self-clearing SmartRef which points to a new array of length 's' filled with copy of optional second argument
    inline SmartRef<C> SmartArray(const size_t& s, const C& TC = {}) { return SmartRef<C>(TC, s); }
    template <class C>  //!< Create self-clearing SmartRef which 'steals' the given reference
    inline SmartRef<C> SmartArray(C* pc) { return SmartRef<C>(pc, true); }
    template <class C>  //!< Create smart reference from value (for using with 'auto')
    inline SmartRef<C> Smartify(const C& c) { return SmartRef<C>(c); }

};  // \ namespace SmartReferenceCounter

#endif /* SMARTREF_HPP_ */
