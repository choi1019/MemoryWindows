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
	virtual void InitializeSuite() {
	}
	virtual void FinalizeSuite() {
	}
	void RunSuite() {
		for (TestCase* pTestCase: m_vPTestCasess) {
			try {
				pTestCase->BeforeInitialize();
				pTestCase->Initialize();
				pTestCase->BeforeRun();
				pTestCase->Run();
				pTestCase->AfterRun();
				pTestCase->Finalize();
				pTestCase->AfterFinalize();
			}
			catch (TestException& exception) {
				exception.Println();
			}
		}
	}
};

