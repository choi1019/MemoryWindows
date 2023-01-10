
#include "Map.h"

/*
template <class KEYTYPE, class VALUETYPE, unsigned MAXLENGTH>
MemoryManager _Map<KEYTYPE, VALUETYPE, MAXLENGTH>::memoryManager(
	sizeof(_Map<KEYTYPE, VALUETYPE, MAXLENGTH>), sizeof(_Map<KEYTYPE, VALUETYPE, MAXLENGTH>));

template class _Map<int, int, 100>;

template <class KEYTYPE, class VALUETYPE, unsigned MAXLENGTH>
inline void *_Map<KEYTYPE, VALUETYPE, MAXLENGTH>::operator new (size_t size) throw() {
	return memoryManager.allocate(size);
}

template <class KEYTYPE, class VALUETYPE, unsigned MAXLENGTH>
inline void _Map<KEYTYPE, VALUETYPE, MAXLENGTH>::operator delete(void *p) throw() {
	memoryManager.free(p);
}

template <class KEYTYPE, class VALUETYPE, unsigned MAXLENGTH>
inline _PairIterator<KEYTYPE, VALUETYPE> _Map<KEYTYPE, VALUETYPE, MAXLENGTH>::find(const KEYTYPE& key) {
	int foundIndex = pIndexTree->search(key);
	if (foundIndex == NOT_FOUND) {
		return this->end();
	}
	return Iterator(this->aryElementPair + foundIndex);
}

template <class KEYTYPE, class VALUETYPE, unsigned MAXLENGTH>
inline _PairIterator<KEYTYPE, VALUETYPE> _Map<KEYTYPE, VALUETYPE, MAXLENGTH>::operator[](const KEYTYPE& key) {
	return this->find(key);
}

template <class KEYTYPE, class VALUETYPE, unsigned MAXLENGTH>
inline void _Map<KEYTYPE, VALUETYPE, MAXLENGTH>::insert(_Map<KEYTYPE, VALUETYPE, MAXLENGTH>::Pair element) throw()
{
	if (this->length == MAXLENGTH) {
		throw static_cast<int>(eOverflow);
	}
	this->aryElementPair[this->length] = element;
	this->pIndexTree->insert(Make_pair<KEYTYPE, int>(element.first, this->length));
	this->length++;
}

template <class KEYTYPE, class VALUETYPE, unsigned MAXLENGTH>
inline void _Map<KEYTYPE, VALUETYPE, MAXLENGTH>::traverse() {
	this->pIndexTree->traverse();
}
*/