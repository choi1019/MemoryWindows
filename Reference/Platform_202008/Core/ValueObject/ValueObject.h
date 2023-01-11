#pragma once
#include "../TypeDef.h"
#define VALUEOBJECT "ValueObject"  // class string name
#define VALUEOBJECT_ID GET_MODULE_ID(EGlobal::eValueObject)  // class int id

#include "../../../../Core_Lib/Source/00Global/ValueObject/ClassInfo.h"
#include "../../../../Core_Lib/Source/00Global/Heap/HeapDynamic.h"

class ValueObject {
private:
	static unsigned gCounter;
	unsigned id;

	ClassInfo classInfo;
public:
	ValueObject(ClassInfo classInfo = ClassInfo(VALUEOBJECT_ID, VALUEOBJECT));
	~ValueObject();

	ClassInfo getClassInfo();
	unsigned getValueObjectId();

	// dynamic memory
	// dynamic memory
	void* operator new(size_t size, void * pHeapDynamic) throw() {
		void* pAllocated = ((HeapDynamic*)pHeapDynamic)->malloc(size);
		return pAllocated;
	}

	void operator delete(void *ptr, void *pHeap) throw() {
//		cout << "Error - ValueObject::delete()\n";
	}
	void operator delete(void *ptr) throw() {
//		cout << "Error - ValueObject::delete()\n";
	}

	void destroy(void *pHeap) {
		this->~ValueObject();
		((HeapDynamic*)pHeap)->free(this);
	}

	virtual void read();
	virtual void store();
};
