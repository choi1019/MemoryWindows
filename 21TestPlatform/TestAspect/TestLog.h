#pragma once

#include <21TestPlatform/typedef.h>
#define _TestLog_Id _GET_TCLASS_UID(_ELayer_TestPlatform::_eTestLog)
#define _TestLog_Name "TestLog"

#include <21TestPlatform/TestObject/TestRoot.h>

class TestLog : public TestRoot
{
private:
	int m_nLogId;

	string m_sObject;
	string m_sFunction;
	string m_sMessage;

public:
	static unsigned s_uCount;
	TestLog(
		string sObject,
		string sFunction,
		string sMessage,
		int nClassId = _TestLog_Id, const char* pcClassName = _TestLog_Name)
		: TestRoot(nClassId, pcClassName)
		, m_nLogId(TestLog::s_uCount++)
		, m_sObject(sObject)
		, m_sFunction(sFunction)
		, m_sMessage(sMessage)
	{
	}
	TestLog(
		int nClassId = _TestLog_Id, const char* pcClassName = _TestLog_Name)
		: TestRoot(nClassId, pcClassName)
		, m_nLogId(TestLog::s_uCount++)
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
			, TestLog::GetTab()
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
		for (unsigned i = 0; i < TestLog::s_uCountTab * SIZE_TAB; i++) {
			TestLog::s_pcTab[i] = SPACE;
		}
		TestLog::s_pcTab[TestLog::s_uCountTab * SIZE_TAB] = '\0';
		return s_pcTab;
	}
	static void AddTab() { TestLog::s_uCountTab++; }
	static void RemoveTab() { TestLog::s_uCountTab--; }
};


#if _DEBUG
#define TESTLOG_HEADER(CLASSNAME, ...) TestLog(CLASSNAME, __VA_ARGS__).PrintHeader()
#define TESTLOG_FOOTER(...) TestLog(__VA_ARGS__).PrintFooter()
#define TESTLOG_NEWLINE(CLASSNAME, ...) TestLog(CLASSNAME, __VA_ARGS__).Println()
#define TESTLOG(CLASSNAME, ...) TestLog(CLASSNAME, __VA_ARGS__).Print()
//	#define LOG_TIME(...) Log(__VA_ARGS__).PrintTime()
#else
#define TESTLOG_HEADER(CLASSNAME, ...)
#define TESTLOG_FOOTER(...)
#define TESTLOG_NEWLINE(CLASSNAME, ...)
#define TESTLOG(CLASSNAME, ...)
//	#define LOG_TIME(...) Log(__VA_ARGS__).PrintTime()
#endif

