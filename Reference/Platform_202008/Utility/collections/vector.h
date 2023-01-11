#pragma once
#include "../TypeDef.h"
#define VECTOR "vector"  // class string name
#define VECTOR_ID GET_MODULE_ID(EUtility::eVector)  // class int id

namespace nibble {
	template <class ELEMENTTYPE>
	class vectorIterator {
	private:
		ELEMENTTYPE* pElement;
	public:
		vectorIterator(ELEMENTTYPE* pELement) : pElement(pELement) {}
		vectorIterator operator++() { vectorIterator current = *this; pElement++; return current; }
		inline bool operator==(const vectorIterator& rhs) { return pElement == rhs.pElement; }
		inline bool operator!=(const vectorIterator& rhs) { return pElement != rhs.pElement; }
		inline ELEMENTTYPE& operator*() { return *pElement; }
		inline ELEMENTTYPE* operator->() { return pElement; }
	};

	template <class ELEMENTTYPE, int MAXLENGTH = 20>
	class vector {
	protected:
		ELEMENTTYPE elements[MAXLENGTH];
		int length;
	public:
		typedef vectorIterator<ELEMENTTYPE> iterator;
		vector(): length(0) {}
		virtual ~vector() {}

		inline iterator begin() { return iterator(this->elements); }
		inline iterator end() { return iterator(this->elements + this->length); }

		inline int size() { return this->length; }
		inline int max_size() { return MAXLENGTH; }
		inline bool empty() { return (length==0)? true: false; }
		inline void clear() { this->length = 0; }
		inline ELEMENTTYPE& operator[](unsigned int index) { return this->elements[index]; }
		inline void push_back(ELEMENTTYPE element) {
			this->elements[this->length] = element;
			this->length++;
		}
	};
}
