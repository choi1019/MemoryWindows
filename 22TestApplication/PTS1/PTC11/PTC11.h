#pragma once

#include "Config.h"
#define _PTC11_ID _GET_TESTCASE_UID(_ELayer_PTS1::_ePTC11)
#define _PTC11_NAME "PTC11"

#include "../../../21TestPlatform/TestCase/TestCase.h"
#include "../../../13PTechnical/PMemoryManager/PMemory.h"
#include "../../../01Base/Aspect/Exception.h"

#include "DomainObject11.h"

class PTC11 : public TestCase {
private:
	DomainObject11* m_pDomainObject;

public:
	PTC11(
		int nClassId = _PTC11_ID,
		const char* pcClassName = _PTC11_NAME)
		: TestCase(nClassId, pcClassName)
		, m_pDomainObject(nullptr)
	{
	}
	virtual ~PTC11() {
	}

	void Initialize() {
		LOG_NEWLINE("new PTC11::PTC11");
		this->m_pDomainObject = new("PTC11::DomainObject") DomainObject11();
		BaseObject::s_pMemory->Show("");
	}
	void Run() {
		// test case
		this->m_pDomainObject->Run();
		BaseObject::s_pMemory->Show("");
	}
	void Finalize() {
		delete this->m_pDomainObject;
		BaseObject::s_pMemory->Show("delete PTC11::m_pDomainObject");
	}

};

