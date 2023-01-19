#pragma once

#include "../typedef.h"
#define _TestLog_Id _GET_CLASS_UID(_ELayer_TestPlatform::_eTestLog)
#define _TestLog_Name "TestLog"

#include "../TestObject/TestRoot.h"


class TestLog : public TestRoot
{
private:
	static unsigned s_uCount;
	int m_nLogId;

	string m_sObject;
	string m_sFunction;
	string m_sMessage;

public:
	TestLog(
		string sObject,
		string sFunction,
		string sMessage,
		int nClassId = _TestLog_Id, const char* pcClassName = _TestLog_Name)
		: TestRoot(nClassId, pcClassName)
		, m_nLogId(s_uCount++)
		, m_sObject(sObject)
		, m_sFunction(sFunction)
		, m_sMessage(sMessage)
	{
	}
	TestLog(
		int nClassId = _TestLog_Id, const char* pcClassName = _TestLog_Name)
		: TestRoot(nClassId, pcClassName)
		, m_nLogId(s_uCount++)
	{
	}
	virtual ~TestLog() {
	}
	virtual void Initialize() {
		TestRoot::Initialize();
	}
	virtual void Finalize() {
		TestRoot::Finalize();
	}

	int GetLogId() { return this->m_nLogId; }

	void Println() {
		printf(
			"\n%sTestLog%d %s::%s %s"
			, this->GetTab()
			, this->GetLogId()
			, m_sObject.c_str()
			, m_sFunction.c_str()
			, m_sMessage.c_str()
			//			,String(m_tTime).c_str()
		);
	}
	void PrintSeparator() {
		printf("\n%s-------------------------------------------------", this->GetTab());
	}

public:
	/////////////////////////////////////////////////
	// pretty formatting
	/////////////////////////////////////////////////
#define COUNT_TAB 20
#define SIZE_TAB 4
#define SPACE ' '

	static unsigned s_uCountTab;
	static char s_pcTab[];
	static char* GetTab() {
		for (unsigned i = 0; i < s_uCountTab * SIZE_TAB; i++) {
			s_pcTab[i] = SPACE;
		}
		s_pcTab[s_uCountTab * SIZE_TAB] = '\0';
		return s_pcTab;
	}
	static void AddTab() { s_uCountTab++; }
	static void RemoveTab() { s_uCountTab--; }
};

