#ifndef REFCOUNTER_HPP_
#define REFCOUNTER_HPP_

namespace SmartReferenceCounter
{
	// Technical virtual base class for proper type preserve
	class Counter
	{
		template <class O>	friend class SmartRef;
		template <class O>	friend class RefCounter;
		size_t count{ 0 };  // <- reference counter
		virtual ~Counter() {};  // <- virtual destructor - that is the purpose of this class existance
	};

	// Technical class for containing object pointer. Provides safe deletion.
	template<class C = void*>
	class RefCounter : public Counter
	{
		template <class O>	friend class SmartRef;
	protected:
		C* objP{ nullptr };  // <- object handle
		bool deletable{ false };  // <- technical data

		RefCounter() { count = 1; }  // default does not use object
		RefCounter(const C& cc, size_t s) : objP(new C[s]), deletable(true) { while (s > 0) objP[--s] = cc; }  // constructing from entity duplicates value
		RefCounter(C* pc) : objP(pc) {}  // construction from reference works with original
		RefCounter(C* pc, int) : objP(pc), deletable(true) {} // array memory handling
		~RefCounter() { if (deletable) delete[] objP; }  // smart deletion
	};

	// Cunning class for nullifying references (like nullptr) in default constructors.
	class NullReference : RefCounter<> { template <class O> friend class SmartRef; } nullRef;

	// Template for smart reference. Provides counter, automatic deletion and type
	// conversion. Designed for easy implementing into code that is already tested
	// and close to completion. SmartRef behaves both like pointer and entity.
	template<class C>
	class SmartRef
	{
		template <class O>    friend class SmartRef;
	protected:
		Counter* refCnt{ nullptr };  // <- actually the only reference in SmartRef

		inline void Copy(const Counter* c)
		{
			decRef();
			++(refCnt = (Counter*)c)->count;
		}

	public:
		SmartRef() { ++(refCnt = &nullRef)->count; }  // default constructor. Uses no object
		SmartRef(C* pc) { ++(refCnt = new RefCounter<C>(pc))->count; } // reference constructor. Counts further refs. No auto deletion
		template <class Temp>
		SmartRef(C* pc, Temp) { ++(refCnt = new RefCounter<C>(pc, 0))->count; } // reference array constructor. Auto deletion provided
		SmartRef(const C& c, const size_t& s = 1) { ++(refCnt = new RefCounter<C>(c, s))->count; } // object/array constructor with initialization. Total memory handling
		SmartRef(const SmartRef<C>& sm) { ++(refCnt = sm.refCnt)->count; } // same-type copy constructor. Provides proper ref counting and memory handling
		template<class Other>
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
		template<class Other>
		SmartRef& operator=(const SmartRef<Other>& sm)  // cross-type assignment operator. Supports copy constructor
		{
			Copy(sm.refCnt);
			return *this;
		}

		inline C& operator *() { return *((RefCounter<C>*)refCnt)->objP; }  // casting to object
		inline C*& operator ->() { return ((RefCounter<C>*)refCnt)->objP; }  // casting to object`s props
		inline operator C&() { return *((RefCounter<C>*)refCnt)->objP; }  // conversion to reference
		inline operator C*&() { return ((RefCounter<C>*)refCnt)->objP; }  // conversion to pointer

		inline size_t getCount() { return refCnt->count; }

		// Dangerous methods!
		inline void incRef() { if (refCnt) ++refCnt->count; } // increasing reference counter
		inline void decRef() { if (refCnt && !(--refCnt->count)) delete refCnt; }  // decreasing reference counter with calling of deletion
	};

	template<class C>
	inline SmartRef<C> SmartArray(const size_t& s, const C& TC = C()) { return SmartRef<C>(TC, s); }
	template<class C>
	inline SmartRef<C> SmartArray(C* pc) { return SmartRef<C>(pc, true); }
};

#endif /* REFCOUNTER_HPP_ */ 