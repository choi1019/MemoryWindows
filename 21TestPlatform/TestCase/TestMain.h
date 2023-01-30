#pragma once

#include <21TestPlatform/typedef.h>
#define _TestMain_Id _GET_TESTCLASS_UID(_ELayer_TestPlatform::_eTestMain)
#define _TestMain_Name "TestMain"

#include <21TestPlatform/TestCase/TestSuite.h>
#include <21TestPlatform/TestAspect/TestException.h>

class TestMain: public TestObject {
private:
	unsigned m_uCurrentIndex;
	unsigned m_uLength;

protected:
	vector<TestSuite*> m_vPTestSuites;

	void add(TestSuite* pTestSuite) {
		this->m_vPTestSuites.push_back(pTestSuite);
	}
public:
	TestMain(int nClassId = _TestMain_Id, const char* pClassName = _TestMain_Name)
		: TestObject(nClassId, pClassName)
		, m_uLength(0)
		, m_uCurrentIndex(0)
		, m_vPTestSuites()
	{
	}
	virtual ~TestMain() {
	}
	void InitializeMain() {
		this->Initialize();
	}
	void FinalizeMain() {
		this->Finalize();
		for (TestSuite* pTestSuite : m_vPTestSuites) {
			delete pTestSuite;
		}
	}
	void RunMain() {
		try {
			for (TestSuite* pTestSuite : m_vPTestSuites) {
				pTestSuite->BeforeInitialize();
				pTestSuite->InitializeSuite();
				pTestSuite->BeforeRun();
				pTestSuite->RunSuite();
				pTestSuite->AfterRun();
				pTestSuite->FinalizeSuite();
				pTestSuite->AfterFinalize();
			}
			this->Run();
		}
		catch (TestException& exception) {
			exception.Println();
		}
	}
};