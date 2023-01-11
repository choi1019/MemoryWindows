#pragma once

#include "Config.h"
#define _PTC22_ID _GET_TESTCASE_UID(_ELayer_PTS2::_ePTC22)
#define _PTC22_NAME "PTC22"

#include "../../../21TestPlatform/TestCase/TestCase.h"
#include "TMemoryManager22.h"

class PTC22 : public TestCase {
private:
	char* m_pMemeoryAllocated;
public:
	PTC22(
		int nClassId = _PTC22_ID,
		const char* pcClassName = _PTC22_NAME)
		: TestCase(nClassId, pcClassName)
		, m_pMemeoryAllocated(nullptr)
	{
	}
	virtual ~PTC22() {
	}
	void Initialize() {
		TestCase::Initialize();

	}

	void Finalize() {
		TestCase::Finalize();
	}

	void Run() {
		size_t szTotalMemory = TMemoryManager22::getMemorySize();
		this->m_pMemeoryAllocated = new char[szTotalMemory];
			Memory::s_pMemoryManager = new(m_pMemeoryAllocated) TMemoryManager22(szTotalMemory);
				Memory::s_pMemoryManager->Initialize();
				Memory::s_pMemoryManager->Show("PTC22::TMemoryManager22");
				Memory::s_pMemoryManager->Finalize();
			delete Memory::s_pMemoryManager;
		delete this->m_pMemeoryAllocated;
	}
};

