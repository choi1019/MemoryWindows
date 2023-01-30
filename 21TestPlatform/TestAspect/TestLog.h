#pragma once

#include <21TestPlatform/typedef.h>
#define _TestLog_Id _GET_TESTCLASS_UID(_ELayer_TestPlatform::_eTestLog)
#define _TestLog_Name "TestLog"

#include <21TestPlatform/TestObject/TestObject.h>

class TestLog : public TestObject
{
public:
	static unsigned s_uCount;
private:
	int m_nLogId;

	string m_sObject;
	string m_sFunction;
	string m_sMessage;

public:
	int GetLogId() { return this->m_nLogId; }

public:
	TestLog(
		string sObject,
		string sFunction,
		string sMessage,
		int nClassId = _TestLog_Id, const char* pcClassName = _TestLog_Name);
	TestLog(
		int nClassId = _TestLog_Id, const char* pcClassName = _TestLog_Name);

	virtual ~TestLog() {
	}
	virtual void Initialize() {
	}
	virtual void Finalize() {
	}

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
	static unsigned s_uCountTab;
	static char s_pcTab[];

	static char* GetTab();
	static void AddTab();
	static void RemoveTab();
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

