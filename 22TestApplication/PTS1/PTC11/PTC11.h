#pragma once

#include <22TestApplication/PTS1/typedef.h>
#define _PTC11_Id _GET_TESTCLASS_UID(_ETestComponent_PS1::_ePTC11)
#define _PTC11_Name "PTC11"

#include <21TestPlatform/TestCase/TestCase.h>

class PTC11 : public TestCase {
private:
//	DomainObject11* m_pDomainObject;

public:
	PTC11(unsigned nClassId = _PTC11_Id, const char* pcClassName = _PTC11_Name);
	virtual ~PTC11();

	void Initialize();
	void Finalize();
	void Run();
};

