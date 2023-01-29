#pragma once

#include <21TestPlatform/typedef.h>
#define _TestObject_Id _GET_CLASS_UID(_ELayer_TestPlatform::_eTestObject)
#define _TestObject_Name "TestObject"

#include <21TestPlatform/TestObject/TestRoot.h>
#include <21TestPlatform/TestAspect/TestException.h>
#include <21TestPlatform/TestAspect/TestLog.h>

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
		TestLog(this->GetClassName(), "Initialize", "���� �մϴ�").Println();
		TestLog().PrintSeparator();
		TestLog::AddTab();
	}
	virtual void Initialize() {
	}
	void BeforeRun() {
		TestLog().PrintSeparator();
		TestLog(this->GetClassName(), "Run", "���� �մϴ�").Println();
		TestLog().PrintSeparator();
		TestLog::AddTab();
	}
	virtual void Run() {
	}
	void AfterRun() {
		TestLog::RemoveTab();
		TestLog().PrintSeparator();
		TestLog(this->GetClassName(), "Run", "���� �Ǿ����ϴ�").Println();
		TestLog().PrintSeparator();
	}
	virtual void Finalize() {
	}
	void AfterFinalize() {
		TestLog::RemoveTab();
		TestLog().PrintSeparator();
		TestLog(this->GetClassName(), "Finalize", "���� �Ǿ����ϴ�").Println();
		TestLog().PrintSeparator();
	}
};

