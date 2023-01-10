#pragma once

#include "../typedef.h"
#define _TestSuite_Id _GET_CLASS_UID(_ELayer_TestPlatform::_eTestSuite)
#define _TestSuite_Name "TestSuite"

#include "TestObject.h"
#include "TestCase.h"

class TestSuite : public TestObject
{
private:
	unsigned m_uCurrentIndex;
	unsigned m_uLength;

protected:
	TestObject* m_aTestObjects[MAX_TESTCASES];

	void add(TestObject* pTestCase) {
		this->m_aTestObjects[m_uCurrentIndex++] = pTestCase;
		this->m_uLength++;
	}

public:
	TestSuite(int nClassId = _TestSuite_Id, const char* pClassName = _TestSuite_Name)
		: TestObject(nClassId, pClassName)
		, m_uLength(0)
		, m_uCurrentIndex(0)
		, m_aTestObjects()
	{
	}
	virtual ~TestSuite() {
		for (unsigned i = 0; i < this->m_uLength; i++) {
			try {
				delete m_aTestObjects[i];
			}
			catch (TestException& exception) {
				exception.Println();
			}
		}

	}
	virtual void Initialize() {
		TestObject::Initialize();
	}
	virtual void Finalize() {
		TestObject::Finalize();
	}
	void Run() {
		for (unsigned i = 0; i < this->m_uLength; i++) {
			TestObject* pTestCase = m_aTestObjects[i];
			try {
				pTestCase->Initialize();
			}
			catch (TestException& exception) {
				exception.Println();
			}

			try {
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

