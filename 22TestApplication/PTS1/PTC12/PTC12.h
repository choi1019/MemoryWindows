#pragma once

#include "Config.h"
#define _PTC12_ID _GET_TESTCASE_UID(_ELayer_PTS1::_ePTC12)
#define _PTC12_NAME "PTC12"

#include "../../../21TestPlatform/TestCase/TestCase.h"
#include "../../../13PTechnical/PMemoryManager/PMemory.h"
#include "../../../01Base/Aspect/Exception.h"

#include "DomainObject.h"

class PTC12 : public TestCase {
public:
	PTC12(
		int nClassId = _PTC12_ID,
		const char* pcClassName = _PTC12_NAME)
		: TestCase(nClassId, pcClassName)
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
		// test case
		DomainObject12* pDomainObject1 = new("DomainObject1") DomainObject12();
		pDomainObject1->Run();
		BaseObject::s_pMemory->Show("");

		DomainObject12* pDomainObject2 = new("DomainObject2") DomainObject12();
		pDomainObject2->Run();
		BaseObject::s_pMemory->Show("");

		delete pDomainObject1;
		BaseObject::s_pMemory->Show("delete pDomainObject1");

		delete pDomainObject2;
		BaseObject::s_pMemory->Show("delete pDomainObject2");
	}
};

