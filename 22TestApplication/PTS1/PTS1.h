#pragma once

#include "Config.h"
#define _PTS1_ID _GET_TESTSUIT_UID(_ELayer_PTestSuit::_ePTS1)
#define _PTS1_NAME "PTS1"

#include "../../21TestPlatform/TestCase/TestSuite.h"

#include "Config.h"
#include "PTC11/PTC11.h"
#include "PTC12/PTC12.h"
#include "PTC13/PTC13.h"

class PTS1: public TestSuite {
private:
	Memory* m_pMemory;
	void* m_pSystemMemeoryAllocated;
	void* m_pUserMemeoryAllocated;

public:
	PTS1(
		unsigned typeId = _PTS1_ID,
		const char* pClassName = _PTS1_NAME)
		: TestSuite(typeId, pClassName)
		, m_pMemory(nullptr)
		, m_pSystemMemeoryAllocated(nullptr)
		, m_pUserMemeoryAllocated(nullptr)
	{
	}
	virtual ~PTS1() {
	}

	virtual void InitializeSuite() {
		try {

			size_t szPage = SIZE_PAGE;
			size_t szSlotUnit = SIZE_SLOT_UNIT;

			// system memory allocation
			size_t szSystemMemory = SIZE_SYSTEM_MEMORY;
			size_t szUserMemory = SIZE_USER_MEMORY;

			m_pSystemMemeoryAllocated = new char[szSystemMemory];
			m_pUserMemeoryAllocated = new char[szUserMemory];

			m_pMemory = new(szSystemMemory, m_pSystemMemeoryAllocated)
				PMemory(m_pUserMemeoryAllocated, szUserMemory, szPage, szSlotUnit);
			BaseObject::s_pMemory = m_pMemory;
			m_pMemory->Initialize();

			m_pMemory->Show("");

			this->add(new PTC11());
			this->add(new PTC12());
			this->add(new PTC13());
		}
		catch (Exception& exception) {
			exception.Println();
		}
	}
	virtual void FinalizeSuite() {
		try {
			DeleteTestCases();

			m_pMemory->Finalize();
			m_pMemory->Show("");

			delete m_pMemory;
			delete m_pUserMemeoryAllocated;
			delete[] m_pSystemMemeoryAllocated;
		}
		catch (Exception& exception) {
			exception.Println();
		}
	}
};

