#pragma once

#include <01Base/typedef.h>
#define _Vector_Id _GET_CLASS_UID(_ELayer_Base::_eVector)
#define _Vector_Name "Vector"

#include <01Base/StdLib/Collection.h>

template <class ELEMENTTYPE>
class VectorIterator {
private:
	ELEMENTTYPE* pElement;
public:
	VectorIterator(ELEMENTTYPE* pELement) : pElement(pELement) {}
	VectorIterator operator++() { VectorIterator current = *this; pElement++; return current; }
	inline bool operator==(const VectorIterator& rhs) { return pElement == rhs.pElement; }
	inline bool operator!=(const VectorIterator& rhs) { return pElement != rhs.pElement; }
	inline ELEMENTTYPE& operator*() { return *pElement; }
	inline ELEMENTTYPE* operator->() { return pElement; }
};

template <class ELEMENTTYPE, int MAXLENGTH = 50>
class Vector: public Collection {
protected:
	ELEMENTTYPE elements[MAXLENGTH];
	int length;
public:
	typedef VectorIterator<ELEMENTTYPE> iterator;
	Vector(
		int nClassId = _Vector_Id,
		const char* pClassName = _Vector_Name)
		: Collection(nClassId, pClassName)
		, elements()
		, length(0) 
	{}
	virtual ~Vector() 
	{}
	virtual void Initialize() { Collection::Initialize(); }
	virtual void Finalize() { Collection::Finalize(); }

	inline iterator begin() { return iterator(this->elements); }
	inline iterator end() { return iterator(this->elements + this->length); }

	inline int Size() { return this->length; }
	inline int Max_size() { return MAXLENGTH; }
	inline bool Empty() { return (length==0)? true: false; }
	inline void Clear() { this->length = 0; }
	inline ELEMENTTYPE& operator[](unsigned int index) { return this->elements[index]; }
	inline bool Add(ELEMENTTYPE element) {
		if (MAXLENGTH == this->length) {
//			throw Exception(1);
			return false;
		}
		this->elements[this->length] = element;
		this->length++;
		return true;
	}
};
