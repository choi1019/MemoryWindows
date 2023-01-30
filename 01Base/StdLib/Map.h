#pragma once

#include <01Base/typedef.h>
#define _Map_Id _GET_CLASS_UID(_ELayer_Base::_eMap)
#define _Map_Name "Map" 

#include "Collection.h"

template <class KEYTYPE, class VALUETYPE>
class MapPair {
public:
	MapPair() { 
		first = KEYTYPE (); 
		second = VALUETYPE();
	}
	MapPair(KEYTYPE key, VALUETYPE value) : first(key), second(value) {}
	inline bool operator==(const MapPair& rhs) { return (first == rhs.first && second == rhs.second); }
	inline bool operator!=(const MapPair& rhs) { return !(first == rhs.first && second == rhs.second); }

	KEYTYPE first;
	VALUETYPE second;
};

template <class KEYTYPE, class VALUETYPE>
class MapIterator {
private:
	MapPair<KEYTYPE, VALUETYPE>* pElement;
public:
	MapIterator(MapPair<KEYTYPE, VALUETYPE>* pElement) : pElement(pElement) {}

	inline MapIterator operator++() {
		MapIterator current = *this; 
		pElement++;
		return current;
	}
	inline MapIterator operator+(const int i) {	return pElement + i; }
	inline MapIterator operator-(const int i) { return pElement - i; }
	inline MapPair<KEYTYPE, VALUETYPE>& operator*() { return *pElement; }
	inline MapPair<KEYTYPE, VALUETYPE>* operator->() { return pElement; }
	inline bool operator==(const MapIterator& rhs) { return pElement == rhs.pElement; }
	inline bool operator!=(const MapIterator& rhs) { return pElement != rhs.pElement; }
};


template <class KEYTYPE, class VALUETYPE, int MAXLENGTH = 100>
class Map: public Collection {

typedef MapIterator<KEYTYPE, VALUETYPE> Iterator;

private:
	int length;
	MapPair<KEYTYPE, VALUETYPE> elements[MAXLENGTH];

public:
	Map(int nClassId = _Map_Id, const char *pcClassName = _Map_Name) 
		: Collection(nClassId, pcClassName)
		, elements()
		, length(0) {}
	virtual ~Map() {}

	virtual void Initialize() {	Collection::Initialize(); }
	virtual void Finalize() { Collection::Finalize(); }

	inline Iterator begin() { return Iterator(this->elements); }
	inline Iterator end() { return Iterator(this->elements + this->length); }

	inline int Size() const { return this->length; }
	inline int Maxsize() const { return MAXLENGTH; }
	inline bool Empty() { return (length==0)? true: false; }
	inline void Clear() { 
		this->length = 0; 
	}
	inline Iterator First() { return Iterator(this->elements); }

	inline Iterator Find(KEYTYPE key) {
		Iterator itr = begin();
		for ( ; itr != end(); ++itr) {
			if (itr->first == key) {
				return itr;
			}
		}
		return itr;
	}

	inline bool Remove(KEYTYPE key) {
		Iterator itrFound = begin();
		for (; itrFound != end(); ++itrFound) {
			if (itrFound->first == key) {
				break;
			}
		}
		// not found
		if (itrFound == this->end()) {
			return false;
		} 
		// found, remove
		else {
			for (; itrFound != end() - 1; ++itrFound) {
				Iterator itrFoundNext = itrFound + 1;
				itrFound->first = itrFoundNext->first;
				itrFound->second = itrFoundNext->second;
			}
			this->length--;
			return true;
		}
	}

	inline bool Add(MapPair<KEYTYPE, VALUETYPE>  mapPair) {
		if (this->length == MAXLENGTH) {
//			throw Exception((int)EError::_eIndexOverflow, "Map", "Add", "eOverFlow");
			return false;
		}
		Iterator itr = this->Find(mapPair.first);

		// found, do not add
		if (itr != this->end()) {
			return false;
		}
		this->elements[this->length] = mapPair;
		this->length++;
		return true;
	}

	inline bool Add(const KEYTYPE& key, VALUETYPE& element)
	{
		MapPair<KEYTYPE, VALUETYPE>  mapPair(key, element);
		return Add(mapPair);
	}

	VALUETYPE& operator[](const KEYTYPE& key)
	{
		Iterator itr = this->Find(key);
		if(itr != this->end()) {
			return itr->second;
		}
		itr->first = key;
		this->length++;
		return itr->second;
	}
};
