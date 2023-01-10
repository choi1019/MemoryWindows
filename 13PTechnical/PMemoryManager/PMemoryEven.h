#pragma once

#include "../typedef.h"
#define _PMemoryEven_Id _GET_CLASS_UID(_ELayer_PTechnical::_ePMemoryEven)
#define _PMemoryEven_Name "PMemoryEven"

#include "../../03Technical/MemoryManager/MemoryEven.h"
#include <Windows.h>

class PMemoryEven : public MemoryEven {
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
	PMemoryEven(
		size_t szSlot,
		size_t uCountTotalSlots,
		int nClassId = _PMemoryEven_Id,
		const char* pcClassName = _PMemoryEven_Name)
		: MemoryEven(szSlot, uCountTotalSlots, nClassId, pcClassName)
	{
		InitializeCriticalSection(&CriticalSection);
	}
	virtual ~PMemoryEven() {
		DeleteCriticalSection(&CriticalSection);
	}

	virtual void Initialize() {
		MemoryEven::Initialize();
	}
	virtual void Finalize() {
		MemoryEven::Finalize();
	}
};