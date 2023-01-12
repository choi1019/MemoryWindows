#pragma once

#include "../typedef.h"
#define _MemoryDynamic_Id _GET_CLASS_UID(_ELayer_Technical::_eMemoryEven)
#define _MemoryDynamic_Name "MemoryDynamic"

#include "Memory.h"
class MemoryDynamic: public Memory
{

public:
	MemoryDynamic(
		size_t szAllocated,
		int nClassId = _MemoryDynamic_Id,
		const char* typeName = _MemoryDynamic_Name)
		: Memory(
			nClassId,
			typeName)
	{
		this->SetSzThis(*(size_t*)((long long)this - sizeof(size_t)));
		this->SetSzAllocated(szAllocated);
		this->SetPAllocated(nullptr);
	}
	virtual ~MemoryDynamic() {
	}

	virtual void Initialize() {
	}
	virtual void Finalize() {
	}

	virtual size_t Show(const char* pTitle) {
	}

protected:

	void* Malloc(size_t szRequested, const char* pcName) {

	}

	void Free(void* pObject) {
	}
};
