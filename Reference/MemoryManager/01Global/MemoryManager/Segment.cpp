#include "Segment.h"

unsigned Segment::sSegmentId = 0;

Segment::Segment(const char *name, char *pStartAddress, unsigned maxSize) :
	id(sSegmentId++), name(name), pStartAddress(pStartAddress), maxSize(maxSize), pCurrentAddress(pStartAddress) {}
Segment::~Segment() {}

void Segment::initialize() {
	this->pCurrentAddress = pStartAddress;
}

void Segment::finalize() {
}

long long Segment::getCurrentSize() {
	return (long long)this->pCurrentAddress - (long long)this->pStartAddress;
}

long Segment::getMaxSize() {
	return this->maxSize;
}

void* Segment::allocate(size_t size) {
	if (this->pCurrentAddress + size > this->pStartAddress + this->maxSize) {
		throw static_cast<int>(eOverflow);
	}
	char* pNewSegment = pCurrentAddress;
	pCurrentAddress = pCurrentAddress + size;

	size_t currentSize = pCurrentAddress - pStartAddress;
	printf("%s::allocated(%zd)-maxSize(%d)\n", this->name, currentSize, maxSize);

	return pNewSegment;
}
