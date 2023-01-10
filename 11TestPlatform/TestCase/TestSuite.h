#pragma once

#include "../typedef.h"
#define _TestSuite_Id _GET_CLASS_UID(_ELayer_TestBase::_eTestSuite)
#define _TestSuite_Name "TestSuite"

#include "TestObject.h"
#include "TestCase.h"

class TestSuite : public TestObject
{
private:
	unsigned m_uCurrentIndex;
	unsigned m_uLength;

protected:
	TestObject* m_aTestCases[MAX_TESTCASES];

	void add(TestObject* pTestCase) {
		this->m_aTestCases[m_uCurrentIndex++] = pTestCase;
		this->m_uLength++;
	}

public:
	TestSuite(int nClassId = _TestSuite_Id, const char* pClassName = _TestSuite_Name)
		: TestObject(nClassId, pClassName)
		, m_uLength(0)
		, m_uCurrentIndex(0)
	{
		memset(m_aTestCases, 0, MAX_TESTCASES);
	}
	virtual ~TestSuite() {
	}
	virtual void Initialize() {
		TestObject::Initialize();
	}
	virtual void Finalize() {
		TestObject::Finalize();
	}
	void Run() {
		for (unsigned i = 0; i < this->m_uLength; i++) {
			TestObject* pTestCase = m_aTestCases[i];
			try {
				pTestCase->Initialize();
				pTestCase->Run();
			}
			catch (TestException& exception) {
				exception.Println();
			}

			try {
				pTestCase->Finalize();
			}
			catch (TestException& exception) {
				exception.Println();
			}
		}
	}
};

