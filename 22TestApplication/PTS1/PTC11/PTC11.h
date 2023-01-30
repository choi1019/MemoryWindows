#pragma once

#include <22TestApplication/PTS1/PTC11/Config.h>
#define _PTC11_Id _GET_TESTCASE_UID(_ELayer_PTS1::_ePTC11)
#define _PTC11_Name "PTC11"

#include <21TestPlatform/TestCase/TestCase.h>
#include <13PTechnical/PMemoryManager/PMemory.h>
#include <22TestApplication/PTS1/PTC11/DomainObject11.h>

class PTC11 : public TestCase {
private:
	DomainObject11* m_pDomainObject;

public:
	PTC11(
		int nClassId = _PTC11_Id,
		const char* pcClassName = _PTC11_Name)
		: TestCase(nClassId, pcClassName)
		, m_pDomainObject(nullptr)
	{
		this->m_pDomainObject = new("PTC11::DomainObject") DomainObject11();
		BaseObject::s_pMemory->Show("");
	}
	virtual ~PTC11() {
		delete this->m_pDomainObject;
		BaseObject::s_pMemory->Show("delete PTC11::m_pDomainObject");
	}

	void Initialize() {
	}
	void Finalize() {
	}
	void Run() {
		// test case
		this->m_pDomainObject->Run();
	}
};

