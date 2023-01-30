#pragma once

#include <21TestPlatform/typedef.h>
#define _TestException_Id _GET_TESTCLASS_UID(_ELayer_TestPlatform::_eTestException)
#define _TestException_Name "TestException"

#include <21TestPlatform/TestObject/TestObject.h>

class TestException : public TestObject 
{
public:
	static unsigned s_uCount;

private:
	int m_nExceptionId;
	int m_nType;
	string m_sObject;
	string m_sFunction;
	string m_sMessage;
public:
	int GetExceptionId() { return this->m_nExceptionId; }

public:
	TestException(
		int nType,
		string sObject,
		string sFunction,
		string sMessage,
		int nClassId = _TestException_Id, const char* pcClassName = _TestException_Name
	);

	~TestException();

	void Initialize();
	void Finalize();
	void Println();
};