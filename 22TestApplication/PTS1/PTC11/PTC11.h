#pragma once

#include "Config.h"
#define _PTC11_ID _GET_TESTCASE_UID(_ELayer_PTS1::_ePTC11)
#define _PTC11_NAME "PTC11"

#include "../../../21TestPlatform/TestCase/TestCase.h"
#include "../../../13PTechnical/PMemoryManager/PMemory.h"
#include "../../../01Base/Aspect/Exception.h"

#include "DomainObject.h"

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
		DomainObject* pDomainObject = new("DomainObject") DomainObject();
		pDomainObject->Show("DomainObject");

		// result
		pMemory->Show("");
		delete pDomainObject;
		pMemory->Show("");
		delete pMemory;

		delete[] this->m_pMemeoryAllocated;
		delete[] pSystemMemoryAllocated;		
	}
};

