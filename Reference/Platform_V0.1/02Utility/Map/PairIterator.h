//////////////////////////////////////////////////////////////
// Created 2017-01-21 by Sungwoon Choi
// All Rights Reserved
//////////////////////////////////////////////////////////////

#ifndef PAIRITERATOR
#define PAIRITERATOR "_PairIterator"
#include "../TypeDef.h"
#define PAIRITERATOR_ID GETCLASSID(_ePairIterator_)

#include "Pair.h"

template <class KEYTYPE, class VALUETYPE>
class _PairIterator {
private:
	typedef _Pair<KEYTYPE, VALUETYPE> Pair;
	Pair* pPair;
public:
	_PairIterator(Pair* pPair) : pPair(pPair) {}
	virtual ~_PairIterator() {}

	inline _PairIterator& operator++() { // ++A
		pPair++;
		return *this;
	}
	inline _PairIterator operator++(int) { // A++
		_PairIterator current = *this;
		++pPair;
		return current;
	}
	inline Pair& operator*() { return *pPair; }
	inline Pair* operator->() { return pPair; }
	inline bool operator==(const _PairIterator& rhs) { return pPair == rhs.pPair; }
	inline bool operator==(_PairIterator& rhs) { return pPair == rhs.pPair; }
	inline bool operator!=(const _PairIterator& rhs) { return pPair != rhs.pPair; }
};
#endif /* PAIRITERATOR */
