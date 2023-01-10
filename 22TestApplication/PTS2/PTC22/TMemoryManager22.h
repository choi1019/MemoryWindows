#pragma once

#include "Config.h"
#define _TMemoryManager22_Id _GET_TESTCASE_UID(_ELayer_PTS2::_eTMemroyManager22)
#define _TMemoryManager22_Name "TMemoryManager"

#include "../../../13PTechnical/PMemoryManager/PMemoryManager.h"

class TMemoryManager22 : public PMemoryManager {

public:
	static size_t getMemorySize() {
		int szTotalMemory1 = sizeof(PMemoryManager) + sizeof(size_t);
		int szTotalMemory2 = sizeof(PMemoryVariable) + PTC22_SIZE_MEMORYBASE + 2 * sizeof(size_t);
		int szTotalMemory3 = sizeof(PMemoryEven) + PTC22_COUNT_EXCEPTION * sizeof(Exception) + 2 * sizeof(size_t);
		int szTotalMemory4 = sizeof(PMemoryEven) + PTC22_COUNT_LOG * sizeof(Log) + 2 * sizeof(size_t);
		//			int szTotalMemory5 = sizeof(PMemoryEven) + PTC11_COUNT_EVENT * sizeof(Event) + 2 * sizeof(size_t);
		//			int szTotalMemory = szTotalMemory1 + szTotalMemory2 + szTotalMemory3 + szTotalMemory4 + szTotalMemory5;
		int szTotalMemory = szTotalMemory1 + szTotalMemory2 + szTotalMemory3 + szTotalMemory4;
		return szTotalMemory;
	}

	TMemoryManager22(
		size_t szTotalMemory,
		unsigned nClassid = _TMemoryManager22_Id,
		const char* pcClassName = _TMemoryManager22_Name)
		: PMemoryManager(
			szTotalMemory,
			nClassid, pcClassName) {

	}
	virtual ~TMemoryManager22() {}


	virtual void Initialize() {
		MemoryManager::Initialize();

		BaseObject::s_pMemory = new("BaseObject::s_pMemory") PMemoryVariable(PTC22_SIZE_MEMORYBASE);
		BaseObject::s_pMemory->Initialize();
		this->Add((Memory*)BaseObject::s_pMemory);

		Exception::s_pMemory = new("Exception::s_pMemoryEven") PMemoryEven(sizeof(Exception), PTC22_COUNT_EXCEPTION);
		Exception::s_pMemory->Initialize();
		this->Add((Memory*)Exception::s_pMemory);

		Log::s_pMemory = new("Log::s_pMemoryEven") PMemoryEven(sizeof(Log), PTC22_COUNT_LOG);
		Log::s_pMemory->Initialize();
		this->Add((Memory*)Log::s_pMemory);

		//		Event::s_pMemory = new("Event::s_pMemoryEven") PMemoryEven(sizeof(Event), countEvent);
		//		Event::s_pMemory->InitializeMemory();
		//		this->Add((Memory*)Event::s_pMemory);

		//		this->Show((String(this->GetClassName()) + "::Initialized").c_str());

	}
	virtual void Finalize() {
		PMemoryManager::Finalize();
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
