//////////////////////////////////////////////////////////////
// Created 2017-01-21 by Sungwoon Choi
// All Rights Reserved
//////////////////////////////////////////////////////////////

#ifndef VECTORITERATOR
#define VECTORITERATOR "_VectorIterator"
#include "../TypeDef.h"
#define VECTORITERATOR_ID GETCLASSID(_eVectorIterator_)

template <class TYPE>
class _VectorIterator {
private:
	TYPE* pElement;
public:
	_VectorIterator(TYPE* pELement) : pElement(pELement) {}
	inline _VectorIterator& operator++() { pElement++; return *this; }
	inline _VectorIterator operator++(int) { _VectorIterator current = *this; pElement++; return current; }
	inline bool operator==(const _VectorIterator& rhs) { return pElement == rhs.pElement; }
	inline bool operator!=(const _VectorIterator& rhs) { return pElement != rhs.pElement; }
	inline TYPE& operator*() { return *pElement; }
	inline TYPE* operator->() { return pElement; }
};

#endif /* _VectorIterator */
