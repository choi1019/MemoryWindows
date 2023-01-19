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
	std::vector<TestObject*> m_vPTestObjects;

	void add(TestObject* pTestCase) {
		this->m_vPTestObjects.push_back(pTestCase);
	}

public:
	TestSuite(int nClassId = _TestSuite_Id, const char* pClassName = _TestSuite_Name)
		: TestObject(nClassId, pClassName)
		, m_uLength(0)
		, m_uCurrentIndex(0)
//		, m_aTestObjects()
	{
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
		for (TestObject* pTestCase: m_vPTestObjects) {
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

