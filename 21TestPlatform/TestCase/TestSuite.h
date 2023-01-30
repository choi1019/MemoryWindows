#pragma once

#include <21TestPlatform/typedef.h>
#define _TestSuite_Id _GET_TESTCLASS_UID(_ELayer_TestPlatform::_eTestSuite)
#define _TestSuite_Name "TestSuite"

#include <vector>
#include <21TestPlatform/TestObject/TestObject.h>
#include <21TestPlatform/TestCase/TestCase.h>
#include <21TestPlatform/TestAspect/TestException.h>

class TestSuite : public TestObject
{
private:
	unsigned m_uCurrentIndex;
	unsigned m_uLength;

protected:
	vector<TestCase*> m_vPTestCasess;

	void add(TestCase* pTestCase) {
		this->m_vPTestCasess.push_back(pTestCase);
	}
	void DeleteTestCases() {
		for (TestCase* pTestCase : m_vPTestCasess) {
			delete pTestCase;
		}
	}
public:
	TestSuite(int nClassId = _TestSuite_Id, const char* pClassName = _TestSuite_Name)
		: TestObject(nClassId, pClassName)
		, m_uLength(0)
		, m_uCurrentIndex(0)
		, m_vPTestCasess()
	{
	}
	virtual ~TestSuite() {

	}
	void InitializeSuite() {
		this->Initialize();
	}
	void FinalizeSuite() {
		this->DeleteTestCases();
		this->Finalize();
	}
	void RunSuite() {
		try {
			for (TestCase* pTestCase: m_vPTestCasess) {
				pTestCase->BeforeInitialize();
				pTestCase->InitializeCase();
				pTestCase->BeforeRun();
				pTestCase->RunCase();
				pTestCase->AfterRun();
				pTestCase->FinalizeCase();
				pTestCase->AfterFinalize();
			}
			this->Run();
		}
		catch (TestException& exception) {
			exception.Println();
		}
	}
};

