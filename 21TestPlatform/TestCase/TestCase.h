#pragma once

#include <21TestPlatform/typedef.h>
#define _TestCase_Id _GET_CLASS_UID(_ELayer_TestPlatform::_eTestCase)
#define _TestCase_Name "TestCase"

#include <21TestPlatform/TestCase/TestObject.h>

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
		const char *pClassName = _TestCase_Name)
		: TestObject(nClassId, pClassName)
	{
	}
	virtual ~TestCase() {}

	virtual void InitializeCase() {
		this->Initialize();
	}
	virtual void FinalizeCase() {
		this->Finalize();
	}
	virtual void RunCase() {
		this->Run();
	}

protected:
	void AssertNotNull(void* pObject) {
		if (pObject == nullptr) {
			throw TestException((unsigned)EAssertion::_eNotNull, this->GetClassName(), __func__, "Test Failed");
		}
	}
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

