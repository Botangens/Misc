#ifndef SMARTREF_HPP_
#define SMARTREF_HPP_

namespace SmartReferenceCounter
{
	// Utility virtual base class for proper type preserve
	class Counter
	{
		template <class O> friend class SmartRef;
		template <class O> friend class RefCounter;
    protected:
		size_t count{ 0 };  // <- reference counter
		virtual ~Counter() {};  // <- virtual destructor - that is the purpose of this class existance
	};

	// Utility class for containing object pointer. Provides safe deletion.
	template<class C = void*>
	class RefCounter : protected Counter
	{
		template <class O> friend class SmartRef;
	protected:
		using ObjectType = C;
		C* objP{ nullptr };  // <- object handle
		bool deletable{ false };  // <- marks if the pointer was 'stolen'

		RefCounter() { count = 1; }  // default does not use object
		RefCounter(const C& cc, size_t s) : objP(new C[s]), deletable(true) { while (s > 0) objP[--s] = cc; }  // constructing from entity duplicates value
		RefCounter(C* pc) : objP(pc) {}  // construction from pointer works with original
		RefCounter(C* pc, int) : objP(pc), deletable(true) {} // array memory handling
		~RefCounter() { if (deletable) delete[] objP; }  // smart deletion
	};

	// Cunning class for nullifying references (like nullptr) in default constructors.
	class NullReference final : RefCounter<> { template <class O> friend class SmartRef; } nullRef;

    // Smart reference. Provides count, automatic deletion and type conversion.
	// Designed for easy implementing into code that is already tested and
	// close to completion. Behaves both like pointer and entity.
	template<class C>
	struct SmartRef
	{
		template <class O> friend class SmartRef;  // Any SmartRef is a friend to any other SmartRef
        using ObjectType = C;
	protected:
		Counter* refCnt{ nullptr };  // <- actually the only data in SmartRef

		inline void Copy(const Counter* c)  // copy the counter (given as const coz calling methods take const refs)
		{
			decRef();
			++(refCnt = (Counter*)c)->count;  // deconst the counter and processing
		}
	public:
		SmartRef() { ++(refCnt = &nullRef)->count; }  // default constructor. Uses no object. Counts as nullRef
		SmartRef(C* pc) { ++(refCnt = new RefCounter<C>(pc))->count; } // pointer constructor. Counts further refs. No auto deletion
		template <class Flag>
		SmartRef(C* pc, Flag) { ++(refCnt = new RefCounter<C>(pc, 0))->count; } // pointer-to-array constructor. Auto deletion provided
		SmartRef(const C& c, const size_t& s = 1) { ++(refCnt = new RefCounter<C>(c, s))->count; } // object/array constructor with initialization. Total memory handling
		SmartRef(const SmartRef<C>& sm) { ++(refCnt = sm.refCnt)->count; } // same-type copy constructor. Provides proper ref counting and memory handling
		template <class Other>
		SmartRef(const SmartRef<Other>& sm) { ++(refCnt = sm.refCnt)->count; } // cross-type copy constructor. Provides proper ref counting and memory handling

		~SmartRef() { decRef(); }  // simple destructor

		SmartRef& operator=(const NullReference& n)
		{
			Copy(&n);
			return *this;
		}
		SmartRef& operator=(const SmartRef& sm)  // same-type assignment operator. Supports copy constructor. MUST BE!
		{
			Copy(sm.refCnt);
			return *this;
		}
		template <class Other>
		SmartRef& operator=(const SmartRef<Other>& sm)  // cross-type assignment operator. Supports copy constructor
		{
			Copy(sm.refCnt);
			return *this;
		}

		explicit inline C& operator *() { return *((RefCounter<C>*)refCnt)->objP; }  // casting to object
		explicit inline C*& operator ->() { return ((RefCounter<C>*)refCnt)->objP; }  // casting to object`s props
		explicit inline operator C&() { return *((RefCounter<C>*)refCnt)->objP; }  // conversion to reference
		explicit inline operator C*&() { return ((RefCounter<C>*)refCnt)->objP; }  // conversion to pointer

		inline size_t getCount() { return refCnt->count; }

		// Dangerous methods! Could be used for low-level control in needed
		inline void incRef() { if (refCnt) ++refCnt->count; } // increasing reference counter
		inline void decRef() { if (refCnt && !(--refCnt->count)) delete refCnt, refCnt = nullptr; }  // decreasing reference counter with calling of deletion
	};

	template <class C>  // Create self-clearing SmartRef which points to a new array of length 's' filled with copy of optional second argument
	inline SmartRef<C> SmartArray(const size_t& s, const C& TC = {}) { return SmartRef<C>(TC, s); }
	template <class C>  // Create self-clearing SmartRef which 'steals' the given reference
	inline SmartRef<C> SmartArray(C* pc) { return SmartRef<C>(pc, true); }
};

#endif /* SMARTREF_HPP_ */ 
