#pragma once

#include <22TestApplication/PTS1/Config.h>
#define _PTS1_ID _GET_TESTSUIT_UID(_ELayer_PTestSuit::_ePTS1)
#define _PTS1_NAME "PTS1"

#include <21TestPlatform/TestCase/TestSuite.h>

#include <22TestApplication/PTS1/PTC11/PTC11.h>
#include <22TestApplication/PTS1/PTC12/PTC12.h>
#include <22TestApplication/PTS1/PTC13/PTC13.h>
#include <22TestApplication/PTS1/PTC14/PTC14.h>

template <int SIZE_SYSTEM_MEMORY, int SIZE_USER_MEMORY, int SIZE_PAGE, int SIZE_SLOT_UNIT>
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

	void Initialize() {
		try {
			// system memory allocation
			size_t szSystemMemory = SIZE_SYSTEM_MEMORY;
			m_pSystemMemeoryAllocated = new char[szSystemMemory];

			// aplication memorty allocation
			size_t szUserMemory = SIZE_USER_MEMORY;
			m_pUserMemeoryAllocated = new char[szUserMemory];

			m_pMemory = new(m_pSystemMemeoryAllocated, szSystemMemory)
				PMemory(m_pUserMemeoryAllocated, szUserMemory, SIZE_PAGE, SIZE_SLOT_UNIT);

			m_pMemory->Initialize();
			m_pMemory->Show("m_pMemory::Initialize()");

			this->add(new("PTC11") PTC11());
			this->add(new("PTC12") PTC12());
			this->add(new("PTC13") PTC13());
			this->add(new("PTC14") PTC14());
			this->add(new("PTC11") PTC11());
			this->add(new("PTC12") PTC12());
		}
		catch (Exception& exception) {
			exception.Println();
			exit(1);
		}
	}
	void Finalize() {
		try {
			m_pMemory->Finalize();
			m_pMemory->GetPPageList()->Show("");
			m_pMemory->Show("");

			delete m_pMemory;
			delete m_pUserMemeoryAllocated;
			delete[] m_pSystemMemeoryAllocated;
		}
		catch (Exception& exception) {
			exception.Println();
			exit(2);
		}
	}
};

