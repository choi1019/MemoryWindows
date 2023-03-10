#pragma once

#include "Config.h"
#define _PTC11_ID _GET_TESTCASE_UID(_ELayer_PTS1::_ePTC11)
#define _PTC11_NAME "PTC11"

#include "../../../21TestPlatform/TestCase/TestCase.h"

#include "TMemoryManager11.h"

class PTC11 : public TestCase {
private:
	char* m_pMemeoryAllocated;
public:
	PTC11(
		int nClassId = _PTC11_ID,
		const char* pcClassName = _PTC11_NAME)
		: TestCase(nClassId, pcClassName)
		, m_pMemeoryAllocated(nullptr)
	{
	}
	virtual ~PTC11() {
	}

	void Initialize() {
		TestCase::Initialize();
	}
	void Finalize() {
		TestCase::Finalize();
	}

	void Run() {
		
		try {
			size_t szTotalMemory = TMemoryManager11::getMemorySize();
			this->m_pMemeoryAllocated = new char[szTotalMemory];
				Memory::s_pMemoryManager = new(m_pMemeoryAllocated) TMemoryManager11(szTotalMemory);
					Memory::s_pMemoryManager->Initialize();
					Memory::s_pMemoryManager->Show("PTC11::TMemoryManager11");
					Memory::s_pMemoryManager->Finalize();
				delete Memory::s_pMemoryManager;
			delete this->m_pMemeoryAllocated;
		}
		catch (Exception& exception) {
			exception.Println();
		}
		
	}
};

