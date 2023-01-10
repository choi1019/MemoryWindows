#pragma once

#include "../typedef.h"
#define _PMemoryManager_Id _GET_CLASS_UID(_ELayer_PTechnical::_ePMemoryManager)
#define _PMemoryManager_Name "PMemoryManager"

#include "../../03Technical/MemoryManager/MemoryManager.h"

#include "PMemoryVariable.h"
#include "PMemoryEven.h"

#include "../../01Base/StdLib/String.h"
#include "../../01Base/Object/BaseObject.h"
#include "../../01Base/Aspect/Exception.h"
#include "../../01Base/Aspect/Log.h"
//#include "../../2Platform/EventQueue/Event.h"

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



	virtual void InitializeMemory(
		size_t szMemoryBase,
		size_t countException,
		size_t countLog,
		size_t countEvent)
	{
		MemoryManager::InitializeMemory();

		BaseObject::s_pMemory = new("BaseObject::s_pMemory") PMemoryVariable(szMemoryBase);
		BaseObject::s_pMemory->InitializeMemory();
		this->Add((Memory*)BaseObject::s_pMemory);

		Exception::s_pMemory = new("Exception::s_pMemoryEven") PMemoryEven(sizeof(Exception), countException);
		Exception::s_pMemory->InitializeMemory();
		this->Add((Memory*)Exception::s_pMemory);

		Log::s_pMemory = new("Log::s_pMemoryEven") PMemoryEven(sizeof(Log), countLog);
		Log::s_pMemory->InitializeMemory();
		this->Add((Memory*)Log::s_pMemory);

//		Event::s_pMemory = new("Event::s_pMemoryEven") PMemoryEven(sizeof(Event), countEvent);
//		Event::s_pMemory->InitializeMemory();
//		this->Add((Memory*)Event::s_pMemory);

		this->Show((String(this->GetClassName()) + "::Initialized").c_str());

	}
	virtual void FinalizeMemory() {
		MemoryManager::FinalizeMemory();

		for (Memory* pMemory : this->GetVMemory()) {
			if (pMemory != nullptr) {
				if (pMemory->GetEState() == Memory::EState::eInitialized) {
					pMemory->FinalizeMemory();
				}
				delete pMemory;
			}
		}

		this->GetVMemory().Clear();
	}

	virtual size_t Show(const char* pMessage) {
		LOG_HEADER(pMessage);

		size_t szMemoryTotal = MemoryStatic::Show("MemoryStatic::s_pMemory");
		size_t szMemoryBase = BaseObject::s_pMemory->Show("BaseObject::s_pMemory");
		size_t szMemoryException = Exception::s_pMemory->Show("Exception::s_pMemory");
		size_t szMemoryLog = Log::s_pMemory->Show("Log::s_pMemory");
//		size_t szMemoryEvent = Event::s_pMemory->Show("Event::s_pMemory");

		size_t szSum = szMemoryBase + szMemoryException + szMemoryLog;
		LOG_FOOTER(pMessage, "sum:", szSum);

		return szSum;
	}
};


