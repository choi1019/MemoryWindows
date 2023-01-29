#pragma once

#include <13PTechnical/typedef.h>
#define _PMemory_Id _GET_CLASS_UID(_ELayer_PTechnical::_ePMemory)
#define _PMemory_Name "PMemory"

#include <03Technical/MemoryManager/Memory.h>

class PMemory : public Memory {
private:
//	CRITICAL_SECTION CriticalSection;

protected:
	virtual void Lock() {
//		EnterCriticalSection(&CriticalSection);
	}
	virtual void UnLock() {
//		LeaveCriticalSection(&CriticalSection);
	}

public:
	PMemory(
		void* pMemeoryAllocated,
		size_t szMemoryAllocated,
		size_t szPage,
		size_t szSlotUnit,

		int nClassId = _PMemory_Id,
		const char* pcClassName = _PMemory_Name)
		: Memory(pMemeoryAllocated, szMemoryAllocated, szPage, szSlotUnit, nClassId, pcClassName)
	{
//		InitializeCriticalSection(&CriticalSection);
	}
	virtual ~PMemory() {
//		DeleteCriticalSection(&CriticalSection);
	}

	virtual void Initialize() {
		Memory::Initialize();
	}
	virtual void Finalize() {
		Memory::Finalize();
	}
};