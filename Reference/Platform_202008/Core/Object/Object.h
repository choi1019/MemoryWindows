#pragma once
#include "../TypeDef.h"
#define OBJECT "Object"  // class string name
#define OBJECT_ID GET_MODULE_ID(EGlobal::eObject)  // class int id

#include "../../../../Core_Lib/Source/00Global/Heap/HeapStatic.h"
#include "../../../../Core_Lib/Source/00Global/ValueObject/ValueObject.h"

class Object : public ValueObject
{
public:
	Object(ClassInfo classInfo = ClassInfo(OBJECT_ID, OBJECT));
	~Object();

	// dynamic memory
	void* operator new(size_t size);
	void operator delete(void* ptr);
};

