//////////////////////////////////////////////////////////////
// Created 2017-01-21 by Sungwoon Choi
// All Rights Reserved
//////////////////////////////////////////////////////////////

#ifndef VECTOR
#define VECTOR "_Vector"
#include "../TypeDef.h"
#define VECTOR_ID GETCLASSID(_eVector_)

#include "VectorIterator.h"

template <class TYPE, unsigned MAXVECTORLENGTH = 64>
class _Vector {
protected:
	TYPE elements[MAXVECTORLENGTH];
	unsigned length;
public:
	typedef _VectorIterator<TYPE> Iterator;

	_Vector() : length(0) {}
	~_Vector() {}

	inline Iterator begin() { return Iterator(this->elements); }
	inline Iterator end() { return Iterator(this->elements + this->length); }

	inline unsigned getLength() { return this->length; }
	inline unsigned getMaxLength() { return MAXVECTORLENGTH; }
	inline bool empty() { return length == 0; }
	inline void clear() { this->length = 0; }
	inline TYPE &operator[](unsigned index) { return this->elements[index];	}
	inline TYPE get(unsigned index) { return this->elements[index]; }
	inline void add(TYPE element) {
		this->elements[this->length] = element;
		this->length++;
	}
};

#endif /* VECTOR_ */
