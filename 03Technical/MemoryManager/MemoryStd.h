#pragma once

#include "../typedef.h"
#define _MemoryStd_Id _GET_CLASS_UID(_ELayer_Base::_eMemoryStd)
#define _MemoryStd_Name "MemoryStd"

#include "Memory.h"
#include <stdlib.h>

class MemoryStd : public Memory
{
protected:
	// critical section
	virtual void Lock() {}
	virtual void UnLock() {}

	// standard c memory manager
	virtual void* Malloc(size_t szAllocate) {
		return ::malloc(szAllocate);
	}
	virtual void Free(void* pObject) {
		::free(pObject);
	}

public:
	// constructors and destructors
	MemoryStd(
		int nClassId = _MemoryStd_Id,
		const char* pClassName = _MemoryStd_Name)
		: Memory(nClassId, pClassName)
	{
	}
	virtual ~MemoryStd()
	{
	}
	virtual void Initialize() {
		RootObject::Initialize();
	}
	virtual void Finalize() {
		RootObject::Finalize();
	}
};

