#pragma once

#include "Config.h"
#define _PTS1_ID _GET_TESTSUIT_UID(_ELayer_PTestSuit::_ePTS1)
#define _PTS1_NAME "PTS1"

#include "../../21TestPlatform/TestCase/TestSuite.h"

#include "Config.h"
#include "PTC11/PTC11.h"
#include "PTC12/PTC12.h"

#define SIZE_SYSTEM_MEMORY 2048
#define SIZE_USER_MEMORY 2048
#define SIZE_PAGE 256
#define SIZE_SLOT_UNIT 8

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
	{
		try {
			size_t szPage = SIZE_PAGE;
			size_t szSlotUnit = SIZE_SLOT_UNIT;

			// system memory allocation
			size_t szSystemMemory = SIZE_SYSTEM_MEMORY;
			m_pSystemMemeoryAllocated = new char[szSystemMemory];
			SystemMemoryObject::s_pSystemMemoryAllocated = m_pSystemMemeoryAllocated;
			SystemMemoryObject::s_pCurrentSystemMemoryAllocated = m_pSystemMemeoryAllocated;

			// user memory allocation
			size_t szUserMemory = SIZE_USER_MEMORY;
			m_pUserMemeoryAllocated = new char[szUserMemory];

			m_pMemory = new("") PMemory(szUserMemory, szPage, szSlotUnit);
			BaseObject::s_pMemory = m_pMemory;

			this->add(new PTC11());
			this->add(new PTC12());
		}
		catch (Exception& exception) {
			exception.Println();
		}
	}


	virtual void Initialize() {
		try {
			TestSuite::Initialize();
			m_pMemory->Initialize(m_pUserMemeoryAllocated);
		}
		catch (Exception& exception) {
			exception.Println();
		}
		}

	virtual void Run() {
		try {
			TestSuite::Run();
		}
		catch (Exception& exception) {
			exception.Println();
		}

	}
	virtual void Finalize() {
		try {
			TestSuite::Finalize();
			m_pMemory->Finalize();
		}
		catch (Exception& exception) {
			exception.Println();
		}
	}

	virtual ~PTS1() {
		try {
			delete m_pMemory;
			delete m_pUserMemeoryAllocated;
			delete[] m_pSystemMemeoryAllocated;
		}
		catch (Exception& exception) {
			exception.Println();
		}
	}
};

