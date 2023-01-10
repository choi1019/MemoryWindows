#pragma once

#include "../typedef.h"
#define _TestObject_Id _GET_CLASS_UID(_ELayer_TestPlatform::_eTestObject)
#define _TestObject_Name "TestObject"

#include "../TestObject/TestRoot.h"
#include "../TestAspect/TestException.h"
#include "../TestAspect/TestLog.h"

class TestObject: public TestRoot
{
private:

public:
	TestObject(
		int nClassId = _TestObject_Id,
		const char *pClassName = _TestObject_Name)
		: TestRoot(nClassId, pClassName)
	{
	}

	virtual ~TestObject() {}

	virtual void Initialize() {
		TestRoot::Initialize();

		TestLog().PrintSeparator();
		TestLog(this->GetClassName(), __func__, "시작 합니다").Println();
		TestLog::AddTab();
	}

	virtual void Finalize() {
		TestLog::RemoveTab();
		TestLog(this->GetClassName(), __func__, "종료 되었습니다").Println();
		TestLog().PrintSeparator();

		TestRoot::Finalize();
	}

	virtual void Run() {}
};

