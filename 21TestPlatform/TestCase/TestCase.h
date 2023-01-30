#pragma once

#include <21TestPlatform/typedef.h>
#define _TestCase_Id _GET_TESTCLASS_UID(_ELayer_TestPlatform::_eTestCase)
#define _TestCase_Name "TestCase"

#include <21TestPlatform/TestObject/TestObject.h>
#include <21TestPlatform/TestAspect/TestException.h>

class TestCase: public TestObject
{
public:
	enum class EAssertion {
		_eBegin = _TestCase_Id,
		_eNotNull,
		_eEqual,
		_eNotEqual,
		_eGT,
		_eEnd
	};

public:
	TestCase(
		int nClassId = _TestCase_Id,
		const char *pClassName = _TestCase_Name);
	virtual ~TestCase();

	virtual void InitializeCase();
	virtual void FinalizeCase();
	virtual void RunCase();

protected:
	void AssertNotNull(void* pObject);
	void AssertEqual(unsigned x, unsigned y);
	void AssertEqual(int x, int y);
	void AssertEqual(long long x, long long y);
};

