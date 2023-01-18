#pragma once

#include "Config.h"
#define _PTC12_ID _GET_TESTCASE_UID(_ELayer_PTS1::_ePTC12)
#define _PTC12_NAME "PTC12"

#include "../../../21TestPlatform/TestCase/TestCase.h"
#include "../../../13PTechnical/PMemoryManager/PMemory.h"
#include "../../../01Base/Aspect/Exception.h"

#include "DomainObject.h"

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
		// system memory allocation
		size_t szSystemMemory = 2048;
		char* pSystemMemoryAllocated = new char[szSystemMemory];
		IMemory::s_pSystemMemoryAllocated = pSystemMemoryAllocated;

		// user memory allocation
		size_t szTotalMemory = 2048;
		this->m_pMemeoryAllocated = new char[szTotalMemory];
		Memory* pMemory = new PMemory(m_pMemeoryAllocated, szTotalMemory);
		BaseObject::s_pMemory = pMemory;

		// test case
		DomainObject12* pDomainObject1 = new("DomainObject1") DomainObject12();
		pDomainObject1->Run();
		pMemory->Show("");

		DomainObject12* pDomainObject2 = new("DomainObject2") DomainObject12();
		pDomainObject2->Run();
		pMemory->Show("");

		delete pDomainObject1;
		pMemory->Show("delete pDomainObject1");

		delete pDomainObject2;
		pMemory->Show("delete pDomainObject2");

		delete pMemory;

		delete[] this->m_pMemeoryAllocated;
		delete[] pSystemMemoryAllocated;		
	}
};

