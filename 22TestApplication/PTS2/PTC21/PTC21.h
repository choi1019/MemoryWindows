#pragma once

#include "Config.h"
#define _PTC21_ID _GET_TESTCASE_UID(_ELayer_PTS2::_ePTC21)
#define _PTC21_NAME "PTC21"

#include "../../../21TestPlatform/TestCase/TestCase.h"

#include "TMemoryManager21.h"

class PTC21 : public TestCase {
private:
	char* m_pMemeoryAllocated;
public:
	PTC21(
		int nClassId = _PTC21_ID,
		const char* pcClassName = _PTC21_NAME)
		: TestCase(nClassId, pcClassName)
		, m_pMemeoryAllocated(nullptr)
	{
	}
	virtual ~PTC21() {
	}

	void Initialize() {
		TestCase::Initialize();
	}
	void Finalize() {
		TestCase::Finalize();
	}

	void Run() {
		
		try {
			size_t szTotalMemory = TMemoryManager21::getMemorySize();
			this->m_pMemeoryAllocated = new char[szTotalMemory];
				Memory::s_pMemoryManager = new(m_pMemeoryAllocated) TMemoryManager21(szTotalMemory);
					Memory::s_pMemoryManager->Initialize();
					Memory::s_pMemoryManager->Show("PTC21::TMemoryManager21");
					Memory::s_pMemoryManager->Finalize();
				delete Memory::s_pMemoryManager;
			delete this->m_pMemeoryAllocated;
		}
		catch (Exception& exception) {
			exception.Println();
		}
		
	}
};

