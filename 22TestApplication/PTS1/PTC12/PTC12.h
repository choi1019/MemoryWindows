#pragma once

#include "Config.h"
#define _PTC12_ID _GET_TESTCASE_UID(_ELayer_PTS1::_ePTC12)
#define _PTC12_NAME "PTC12"

#include "../../../21TestPlatform/TestCase/TestCase.h"
#include "../../../13PTechnical/PMemoryManager/PMemory.h"
#include "../../../01Base/Aspect/Exception.h"

#include "DomainObject12.h"

class PTC12 : public TestCase {
private:
	DomainObject12* m_pDomainObject1;
	DomainObject12* m_pDomainObject2;
public:
	PTC12(
		int nClassId = _PTC12_ID,
		const char* pcClassName = _PTC12_NAME)
		: TestCase(nClassId, pcClassName)
		, m_pDomainObject1(nullptr)
		, m_pDomainObject2(nullptr)
	{
	}
	virtual ~PTC12() {
	}
	void Initialize() {
		LOG_NEWLINE("new PTC12::PTC12");
		m_pDomainObject1 = new("PTC12::DomainObject1") DomainObject12();
		BaseObject::s_pMemory->Show("");
		m_pDomainObject2 = new("PTC12::DomainObject2") DomainObject12();
		BaseObject::s_pMemory->Show("");
	}
	void Finalize() {
		delete m_pDomainObject1;
		BaseObject::s_pMemory->Show("delete PTC12::m_pDomainObject1");
		delete m_pDomainObject2;
		BaseObject::s_pMemory->Show("delete PTC12::m_pDomainObject2");
	}
	void Run() {
		// test case
		m_pDomainObject1->Run();
		m_pDomainObject2->Run();
		BaseObject::s_pMemory->Show("");
	}
};

