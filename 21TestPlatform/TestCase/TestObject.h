#pragma once

#include "../typedef.h"
#define _TestObject_Id _GET_CLASS_UID(_ELayer_TestPlatform::_eTestObject)
#define _TestObject_Name "TestObject"

#include "../TestObject/TestRoot.h"
#include "../TestAspect/TestException.h"
#include "../TestAspect/TestLog.h"

class TestObject: public TestRoot
{
public:
	TestObject(
		int nClassId = _TestObject_Id,
		const char *pClassName = _TestObject_Name)
		: TestRoot(nClassId, pClassName)
	{
	}
	virtual ~TestObject() {}

	void BeforeInitialize() {
		TestLog().PrintSeparator();
		TestLog(this->GetClassName(), "Initialize", "시작 합니다").Println();
		TestLog::AddTab();
	}
	virtual void Initialize() {
	}
	void BeforeRun() {
		TestLog().PrintSeparator();
		TestLog(this->GetClassName(), "Run", "시작 합니다").Println();
		TestLog::AddTab();
	}
	virtual void Run() {
	}
	void AfterRun() {
		TestLog::RemoveTab();
		TestLog(this->GetClassName(), "Run", "종료 되었습니다").Println();
		TestLog().PrintSeparator();
	}
	virtual void Finalize() {
	}
	void AfterFinalize() {
		TestLog::RemoveTab();
		TestLog(this->GetClassName(), "Finalize", "종료 되었습니다").Println();
		TestLog().PrintSeparator();
	}
};

