#pragma once

#include "../typedef.h"
#define _PMemoryVariable_Id _GET_CLASS_UID(_ELayer_PTechnical::_ePMemoryVariable)
#define _PMemoryVariable_Name "PMemoryVariable"

#include "../../03Technical/MemoryManager/MemoryDynamic.h"

class PMemoryDynamic : public MemoryDynamic {
private:
	CRITICAL_SECTION CriticalSection;

protected:
	virtual void Lock() {
		EnterCriticalSection(&CriticalSection);
	}
	virtual void UnLock() {
		LeaveCriticalSection(&CriticalSection);
	}

public:
	PMemoryDynamic(
		size_t szAllocated,
		int nClassId = _PMemoryVariable_Id,
		const char* pcClassName = _PMemoryVariable_Name)
		: MemoryDynamic(szAllocated, nClassId, pcClassName)
	{
		InitializeCriticalSection(&CriticalSection);
	}
	virtual ~PMemoryDynamic() {
		DeleteCriticalSection(&CriticalSection);
	}

	virtual void Initialize() {
		MemoryDynamic::Initialize();
	}
	virtual void Finalize() {
		MemoryDynamic::Finalize();
	}
};