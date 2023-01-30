#pragma once

#include <21TestPlatform/typedef.h>
#define _TestRoot_id _GET_TESTCASE_UID(_ELayer_TestPlatform::_eTestRoot)
#define _TestRoot_Name "TestRoot"

#define MAX_TESTCASES 20

class TestRoot {
public:
	TestRoot(
		int nClassId = _TestRoot_id,
		const char* pcClassName = _TestRoot_Name)
	{
	}
	virtual ~TestRoot() {}

	virtual void Initialize() {}
	virtual void Finalize() {}
};
