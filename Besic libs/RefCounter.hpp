#ifndef REFCOUNTER_HPP_
#define REFCOUNTER_HPP_

namespace SmartReferenceCounter
{
	// technical class for containing object pointer and reference counter.
	// Provides safe deletion.
	class RefCounter
	{
		template <class C>	friend class SmartRef;
	protected:
		void* objP{ nullptr };  // <- object handle
		size_t count{ 0 };  // <- reference counter
		bool deletable{ false };  // <- technical data

		static RefCounter nullCtr;

		RefCounter() : count(1) {}  // default does not use object
		template<class C>
		RefCounter(const C& cc, size_t s) : objP(new C[s]), deletable(true)
			{ while (s > 0) ((C*)objP)[--s] = cc; }  // constructing from entity duplicates value
		template<class C>
		RefCounter(C* pc) : objP(pc) {}  // construction from reference works with original
		template<class C>
		RefCounter(C* pc, int) : objP(pc), deletable(true) {} // array memory handling
		~RefCounter() { if (deletable) delete[] objP; }  // smart deletion
	};
	RefCounter RefCounter::nullCtr;

	// template for smart reference. Provides counter, automatic deletion and type
	// conversion. Designed for easy implementing into code that is already tested
	// and close to completion. SmartRef behaves both like pointer and entity.
	template<class C>
	class SmartRef
	{
		template <class C>	friend class SmartRef;
	protected:
		RefCounter* refCnt{ nullptr };  // <- actually the only reference in SmartRef

		template<class O = C>
		inline void Copy(const SmartRef<O>& sm)
		{
			decRef();
			++(refCnt = sm.refCnt)->count;
		}

	public:
		SmartRef() { ++(refCnt = &RefCounter::nullCtr)->count; }  // default constructor. Uses no object
		SmartRef(C* pc) { ++(refCnt = new RefCounter(pc))->count; } // reference constructor. Counts further refs. No auto deletion

		template <class Temp>
		SmartRef(C* pc, Temp) { ++(refCnt = new RefCounter(pc, 0))->count; } // reference array constructor. Auto deletion provided
		SmartRef(const C& c, const size_t& s = 1) { ++(refCnt = new RefCounter(c, s))->count; } // object/array constructor with initialization. Total memory handling
		SmartRef(const SmartRef<C>& sm) { ++(refCnt = sm.refCnt)->count; } // same-type copy constructor. Provides proper ref counting and memory handling

		template<class Other>
		SmartRef(const SmartRef<Other>& sm) { ++(refCnt = sm.refCnt)->count; } // cross-type copy constructor. Provides proper ref counting and memory handling

		~SmartRef() { decRef(); }  // simple destructor

		SmartRef& operator=(SmartRef sm)  // same-type assignment operator. Supports copy constructor. MUST BE!
		{
			Copy(sm);
			return *this;
		}
		template<class Other>
		SmartRef& operator=(SmartRef<Other> sm)  // cross-type assignment operator. Supports copy constructor
		{
			Copy(sm);
			return *this;
		}

		inline C& operator *() { return *(C*)refCnt->objP; }  // casting to object
		//inline C* operator &() { return (C*)refCnt->objP; }  // casting to object reference
		inline C* operator ->() { return (C*)refCnt->objP; }  // casting to object`s props
		inline operator C&() { return *(C*)refCnt->objP; }  // conversion to reference
		inline operator C*() { return (C*)refCnt->objP; }  // conversion to pointer
		inline SmartRef<C>* Ptr() { return this; }  // get own address

		inline size_t getCount() { return refCnt->count; }

		// Dangerous methods!
		inline void incRef() { if (refCnt) ++refCnt->count; } // increasing reference counter
		inline void decRef() { if (refCnt && !(--refCnt->count)) delete refCnt; }  // decreasing reference counter with calling of deletion
	};

	template<class C>
	inline SmartRef<C> SmartArray(const size_t& s, const C& TC = C()) { return SmartRef<C>(TC, s); }
	template<class C>
	inline SmartRef<C> SmartArray(C* pc) { return SmartRef<C>(pc, true); }
	template<class C>
	inline SmartRef<C>* Ptr(SmartRef<C>& sr) { return sr.Ptr(); }
};

#endif /* REFCOUNTER_HPP_ */