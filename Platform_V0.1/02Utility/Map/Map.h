//////////////////////////////////////////////////////////////
// Created 2017-01-21 by Sungwoon Choi
// All Rights Reserved
//////////////////////////////////////////////////////////////

#ifndef MAP
#define MAP "_Map"
#include "../TypeDef.h"
#define MAP_ID GETCLASSID(_eMap_)

#include "PairIterator.h"

template <class KEYTYPE, class VALUETYPE, unsigned MAXLENGTH = 50>
class _Map {
public:
	typedef _PairIterator<KEYTYPE, VALUETYPE> Iterator;
	typedef _Pair<KEYTYPE, VALUETYPE>  Pair;

private:
	// number of elements
	unsigned length;
	// real data element store
	Pair aryElementPair[MAXLENGTH];

public:

	inline unsigned getLength() const { return this->length; }
	inline unsigned getMaxLength() const { return MAXLENGTH; }
	inline bool empty() { return length == 0; }
	inline void clear() { this->length = 0; }

	inline Iterator first() { return Iterator(this->aryElementPair); }
	inline Iterator begin() { return Iterator(this->aryElementPair); }
	inline Iterator end() { return Iterator(this->aryElementPair + this->length); }

	_Map() : length(0) {
	}
	virtual ~_Map() {
	}

	inline Iterator find(const KEYTYPE& key) {
		for (Pair *current = aryElementPair; current < aryElementPair+length; current++) {
			if (current->first == key) {
				return Iterator(current);
			}
		}
		return Iterator(this->aryElementPair + this->length);
	}
	inline Iterator& operator[](const KEYTYPE& key) {
		return this->find(key);
	}
	void insert(_Map<KEYTYPE, VALUETYPE, MAXLENGTH>::Pair element)
	{
		if (this->length == MAXLENGTH) {
//			throw static_cast<int>(eOverflow);
		}
		for (unsigned i = 0; i < this->length; i++) {
			if (this->aryElementPair[i].first == element.first && this->aryElementPair[i].second == element.second) {
				return;
			}
		}
		this->aryElementPair[this->length] = element;
		this->length++;
	}
};

#endif // MAP
