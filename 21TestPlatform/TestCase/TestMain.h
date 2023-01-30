#pragma once

#include <21TestPlatform/typedef.h>
#define _TestMain_Id _GET_TESTCLASS_UID(_ELayer_TestPlatform::_eTestMain)
#define _TestMain_Name "TestMain"

#include <21TestPlatform/TestCase/TestSuite.h>

class TestMain: public TestSuite {
public:
	TestMain(unsigned nClassId = _TestMain_Id, const char* pClassName = _TestMain_Name);
	virtual ~TestMain();
	void InitializeMain();
	void FinalizeMain();
	void RunMain();
};