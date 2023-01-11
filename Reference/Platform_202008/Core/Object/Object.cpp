#include "Object.h"

Object::Object( ClassInfo classInfo) 
	: ValueObject(classInfo)
{
}

Object::~Object() {
}

// dynamic memory
void* Object::operator new(size_t size) {
	void *pAllocated = HeapStatic::spHeapStatic->malloc(size);
	return pAllocated;
}

void Object::operator delete(void* ptr) {
//	cout << "Error - Object::delete()\n";
	HeapStatic::spHeapStatic->free(ptr);
}