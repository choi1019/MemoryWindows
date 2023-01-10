#pragma once

#include "../typedef.h"
#define _TestCase_Id _GET_CLASS_UID(_ELayer_TestPlatform::_eTestCase)
#define _TestCase_Name "TestCase"

#include "TestObject.h"
#include "../TestAspect/TestException.h"
#include "../TestAspect/TestLog.h"
//#undef GetClassName

class TestCase: public TestObject
{
public:
	enum class EAssertion {
		_eBegin = _TestCase_Id,
		_eEqual,
		_eNotEqual,
		_eGT,
		_eEnd
	};

public:
	TestCase(
		int nClassId = _TestCase_Id,
		const char *pClassName = _TestCase_Name)
		: TestObject(nClassId, pClassName)
	{
	}
	virtual ~TestCase() {}

	virtual void Initialize() {
		TestObject::Initialize();
	}

	virtual void Finalize() {
		TestObject::Finalize();
	}

	virtual void Run() {
		TestObject::Run();
	}

protected:
	void AssertEqual(unsigned x, unsigned y) {
		if (x != y) {
			throw TestException((unsigned)EAssertion::_eEqual, this->GetClassName(), __func__, "Test Failed");
		}
	}
	void AssertEqual(int x, int y) {
		if (x != y) {
			throw TestException((unsigned)EAssertion::_eEqual, this->GetClassName(), __func__, "Test Failed");
		}
	}
	void AssertEqual(long long x, long long y) {
		if (x != y) {
			throw TestException((unsigned)EAssertion::_eEqual, this->GetClassName(), __func__, "Test Failed");
		}
	}
};

