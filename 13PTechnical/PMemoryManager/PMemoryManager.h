#pragma once

#include "../typedef.h"
#define _PMemoryManager_Id _GET_CLASS_UID(_ELayer_PTechnical::_ePMemoryManager)
#define _PMemoryManager_Name "PMemoryManager"

#include "../../03Technical/MemoryManager/MemoryManager.h"

#include "PMemoryDynamic.h"
//#include "PMemoryEven.h"

#include <Windows.h>
#undef GetClassName

class PMemoryManager : public MemoryManager
{
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

	PMemoryManager(
		size_t szTotalMemory,
		unsigned nClassid = _PMemoryManager_Id,
		const char* pcClassName = _PMemoryManager_Name)
		: MemoryManager(
			szTotalMemory,
			nClassid, pcClassName)
	{
		InitializeCriticalSection(&CriticalSection);
	}
	virtual ~PMemoryManager() 
	{
		DeleteCriticalSection(&CriticalSection);
	}



	virtual void Initialize() {}

	virtual void Finalize() {
		MemoryManager::Finalize();

		for (Memory* pMemory : this->GetVMemory()) {
			if (pMemory != nullptr) {
				if (pMemory->GetEState() == Memory::EState::eInitialized) {
					pMemory->Finalize();
				}
				delete pMemory;
			}
		}

		this->GetVMemory().Clear();
	}
};


