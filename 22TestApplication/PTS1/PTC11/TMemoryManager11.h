#pragma once

#include "Config.h"
#define _TMemoryManager11_Id _GET_TESTCASE_UID(_ELayer_PTS1::_eTMemroyManager11)
#define _TMemoryManager11_Name "TMemoryManager"

#include "../../../13PTechnical/PMemoryManager/PMemoryManager.h"


class TMemoryManager11 : public PMemoryManager {

public:
	static size_t getMemorySize() {
		int szTotalMemory = 1000000;
		return szTotalMemory;
	}

	TMemoryManager11(
		size_t szTotalMemory,
		int nClassid = _TMemoryManager11_Id,
		const char* pcClassName = _TMemoryManager11_Name)
		: PMemoryManager(
			szTotalMemory,
			nClassid, pcClassName) {

	}
	virtual ~TMemoryManager11() {}


	virtual void Initialize() {
		MemoryManager::Initialize();

		BaseObject::s_pMemory = new("BaseObject::s_pMemory") PMemoryVariable(PTC11_SIZE_MEMORYBASE);
		BaseObject::s_pMemory->Initialize();
		this->Add((Memory*)BaseObject::s_pMemory);
	}

	virtual void Finalize() {
		PMemoryManager::Finalize();
	}

	virtual size_t Show(const char* pMessage) {
		Log::AddTab();
		LOG_HEADER(pMessage);

		LOG_FOOTER(pMessage, "sum:", "");
		Log::RemoveTab();
		return 0;
	}
};
