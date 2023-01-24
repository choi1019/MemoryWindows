#pragma once

#include "Config.h"
#define _PTC14_ID _GET_TESTCASE_UID(_ELayer_PTS1::_ePTC14)
#define _PTC14_NAME "PTC14"

#include "../../../21TestPlatform/TestCase/TestCase.h"
#include "../../../13PTechnical/PMemoryManager/PMemory.h"
#include "../../../01Base/Aspect/Exception.h"

#include "DomainObject14.h"

class PTC14 : public TestCase {
private:
	DomainObject14* m_pDomainObject1;
	DomainObject14* m_pDomainObject2;
public:
	PTC14(
		int nClassId = _PTC14_ID,
		const char* pcClassName = _PTC14_NAME)
		: TestCase(nClassId, pcClassName)
		, m_pDomainObject1(nullptr)
		, m_pDomainObject2(nullptr)
	{
		LOG_NEWLINE("new PTC14::PTC14");
		m_pDomainObject1 = new("PTC14::DomainObject1") DomainObject14();
		AssertNotNull(m_pDomainObject1);

		BaseObject::s_pMemory->Show("");
		m_pDomainObject2 = new("PTC14::DomainObject2") DomainObject14();
		AssertNotNull(m_pDomainObject2);

		BaseObject::s_pMemory->Show("");
	}
	virtual ~PTC14() {
		BaseObject::s_pMemory->Show("delete PTC14::m_pDomainObject1");
		delete m_pDomainObject1;
		BaseObject::s_pMemory->Show("delete PTC14::m_pDomainObject1");
		delete m_pDomainObject2;
		BaseObject::s_pMemory->Show("delete PTC14::m_pDomainObject2");
	}
	void Initialize() {
	}
	void Finalize() {
	}

	void Run() {
		// test case
		m_pDomainObject1->Run();
		m_pDomainObject2->Run();
		BaseObject::s_pMemory->Show("");
	}

};

