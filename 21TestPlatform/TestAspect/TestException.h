#pragma once

#include <21TestPlatform/typedef.h>
#define _TestException_Id _GET_CLASS_UID(_ELayer_TestPlatform::_eTestException)
#define _TestException_Name "TestException"

#include <21TestPlatform/TestObject/TestRoot.h>

class TestException : public TestRoot
{
private:
	static unsigned s_uCount;
	int m_nExceptionId;

	int m_nType;
	string m_sObject;
	string m_sFunction;
	string m_sMessage;
public:

	TestException(
		int nType,
		string sObject,
		string sFunction,
		string sMessage,
		int nClassId = _TestException_Id, const char* pcClassName = _TestException_Name)
		: TestRoot(nClassId, pcClassName)
		, m_nExceptionId(s_uCount++)
		, m_nType(nType)
		, m_sObject(sObject)
		, m_sFunction(sFunction)
		, m_sMessage(sMessage)
	{
	}
	virtual ~TestException() {}

	virtual void Initialize() {
		TestRoot::Initialize();
	}
	virtual void Finalize() {
		TestRoot::Finalize();
	}

	int GetExceptionId() { return this->m_nExceptionId; }

	void Println() {
		printf(
			">> TestException(%d) %s::%s %s\n"
			, m_nType
			, m_sObject.c_str()
			, m_sFunction.c_str()
			, m_sMessage.c_str()
			//			,String(m_tTime).c_str()
		);
	}
};