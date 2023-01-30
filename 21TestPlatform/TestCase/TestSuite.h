#pragma once

#include <21TestPlatform/typedef.h>
#define _TestSuite_Id _GET_TESTCLASS_UID(_ELayer_TestPlatform::_eTestSuite)
#define _TestSuite_Name "TestSuite"

#include <vector>
#include <21TestPlatform/TestObject/TestObject.h>

class TestSuite : public TestObject
{
private:
	unsigned m_uCurrentIndex;
	unsigned m_uLength;

protected:
	vector<TestObject*> m_vPTestObjects;

	void add(TestObject* pTestCase);
	void DeleteTestCases();
	
public:
	TestSuite(unsigned nClassId = _TestSuite_Id, const char* pClassName = _TestSuite_Name);
	virtual ~TestSuite();
	void InitializeSuite();
	void FinalizeSuite();
	void RunSuite();
};

