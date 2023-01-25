#pragma once

#include "../typedef.h"
#define _TestSuite_Id _GET_CLASS_UID(_ELayer_TestPlatform::_eTestSuite)
#define _TestSuite_Name "TestSuite"

#include "../../01Base/Aspect/Exception.h"

#include "TestObject.h"
#include "TestCase.h"
#include <vector>

class TestSuite : public TestObject
{
private:
	unsigned m_uCurrentIndex;
	unsigned m_uLength;

protected:
	std::vector<TestCase*> m_vPTestCasess;

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
		this->Finalize();
		this->DeleteTestCases();
	}
	void RunSuite() {
		try {
			for (TestCase* pTestCase: m_vPTestCasess) {
				pTestCase->BeforeInitialize();
				pTestCase->InitializeCase();
			}
			for (TestCase* pTestCase : m_vPTestCasess) {
				pTestCase->BeforeRun();
				pTestCase->RunCase();
				pTestCase->AfterRun();
			}
			for (TestCase* pTestCase : m_vPTestCasess) {
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

