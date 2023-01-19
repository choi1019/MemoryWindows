
#pragma once

#include "Config.h"
#define _PTC11_ID _GET_TESTCASE_UID(_ELayer_PTS1::_ePTC11)
#define _PTC11_NAME "PTC11"

#include "../../../21TestPlatform/TestCase/TestCase.h"
#include "../../../13PTechnical/PMemoryManager/PMemory.h"
#include "../../../01Base/Aspect/Exception.h"

#include "DomainObject.h"

class PTC11 : public TestCase {
public:
	PTC11(
		int nClassId = _PTC11_ID,
		const char* pcClassName = _PTC11_NAME)
		: TestCase(nClassId, pcClassName)
	{
	}
	virtual ~PTC11() {
	}

	void Initialize() {
		TestCase::Initialize();
		LOG_TIME("PTC11");
	}
	void Finalize() {
		TestCase::Finalize();
		LOG_TIME("PTC11");
	}

	void Run() {
		// test case
		LOG_NEWLINE("new DomainObject11");
		DomainObject11* pDomainObject = new("DomainObject") DomainObject11();
		pDomainObject->Initialize();
		pDomainObject->Run();
		BaseObject::s_pMemory->Show("");

		pDomainObject->Finalize();
		LOG_NEWLINE("delete DomainObject11");
		delete pDomainObject;
		BaseObject::s_pMemory->Show("");
	}
};

