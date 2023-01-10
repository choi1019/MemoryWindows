#pragma once

#include "Config.h"
#define _PTC12_ID _GET_TESTCASE_UID(_ELayer_PTS1::_ePTC12)
#define _PTC12_NAME "PTC12"

#include "../../../21TestPlatform/TestCase/TestCase.h"
#include "TMemoryManager12.h"

class PTC12 : public TestCase {
private:
	char* m_pMemeoryAllocated;
public:
	PTC12(
		int nClassId = _PTC12_ID,
		const char* pcClassName = _PTC12_NAME)
		: TestCase(nClassId, pcClassName)
		, m_pMemeoryAllocated(nullptr)
	{
	}
	virtual ~PTC12() {
	}
	void Initialize() {
		TestCase::Initialize();

	}

	void Finalize() {
		TestCase::Finalize();
	}

	void Run() {
		try {
			size_t szTotalMemory = TMemoryManager12::getMemorySize();
			this->m_pMemeoryAllocated = new char[szTotalMemory];
				Memory::s_pMemoryManager = new(m_pMemeoryAllocated) TMemoryManager12(szTotalMemory);
					Memory::s_pMemoryManager->Initialize();
					Memory::s_pMemoryManager->Show("PTC12::TMemoryManager12");
					Memory::s_pMemoryManager->Finalize();
				delete Memory::s_pMemoryManager;
			delete this->m_pMemeoryAllocated;
		}
		catch (Exception& exception) {
			exception.Println();
		}
			
	}
};

