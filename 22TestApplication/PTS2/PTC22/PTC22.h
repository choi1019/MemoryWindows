#pragma once

#include "Config.h"
#define _PTC22_ID _GET_TESTCASE_UID(_ELayer_PTS2::_ePTC22)
#define _PTC22_NAME "PTC22"

#include "../../../21TestPlatform/TestCase/TestCase.h"

class PTC22 : public TestCase {
private:
	char* m_pMemeoryAllocated;
public:
	PTC22(
		int nClassId = _PTC22_ID,
		const char* pcClassName = _PTC22_NAME)
		: TestCase(nClassId, pcClassName)
		, m_pMemeoryAllocated(nullptr)
	{
	}
	virtual ~PTC22() {
	}
	void Initialize() {
		TestCase::Initialize();

	}

	void Finalize() {
		TestCase::Finalize();
	}

	void Run() {
	}
};

