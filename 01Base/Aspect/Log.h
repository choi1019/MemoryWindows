#pragma once

#include <01Base/typedef.h>
#define _Log_Id _GET_CLASS_UID(_ELayer_Base::_eLog)
#define _Log_Name "Log"

#include <01Base/Aspect/Aspect.h>
#include <01Base/StdLib/String.h>

class Log : public Aspect
{
protected:
	String m_sObject;
	String m_sFunction;
	String m_sMessage1;
	String m_sMessage2;
	String m_sMessage3;
//	time_t m_tTime;

public:
	String GetSource() { return this->m_sObject; }
	String GetFunction() { return this->m_sFunction; }
	String GetMessage1() { return this->m_sMessage1; }
	String GetMessage2() { return this->m_sMessage2; }
	String GetMessage3() { return this->m_sMessage3; }
	//	String GetTime() { return String(m_tTime); }

public:
	Log(String sObject = "",
		String sFunction = "",
		String sMessage1 = "",
		String sMessage2 = "",
		String sMessage3 = "",
		unsigned classId = _Log_Id,
		const char* pClassName = _Log_Name)
		: Aspect(classId, pClassName)
		, m_sObject(sObject)
		, m_sFunction(sFunction)
		, m_sMessage1(sMessage1)
		, m_sMessage2(sMessage2)
		, m_sMessage3(sMessage3)

	{
//		m_tTime = time(0);

	}
	virtual ~Log() {}

	virtual void Initialize() {
		Aspect::Initialize();
	}
	virtual void Finalize() {
		Aspect::Finalize();
	}
	/*
	void PrintTime() {
		printf(
			"\n%s%s: %s %s %s %s"
			, this->GetTab()
			, String(m_tTime).c_str()
			, m_sObject.c_str()
			, m_sFunction.c_str()
			, m_sMessage1.c_str()
			, m_sMessage2.c_str()
		);
	}
	*/
	void Print() {
		printf(
			"%s%s %s %s %s %s"
			, Aspect::GetTab()
			, m_sObject.c_str()
			, m_sFunction.c_str()
			, m_sMessage1.c_str()
			, m_sMessage2.c_str()
			, m_sMessage3.c_str()
			//			,String(m_tTime).c_str()
		);
	}
	void Println() {
		printf(
			"\n%s%s %s %s %s %s"
			, Aspect::GetTab()
			,m_sObject.c_str()
			,m_sFunction.c_str()
			, m_sMessage1.c_str()
			, m_sMessage2.c_str()
			, m_sMessage3.c_str()
			///			,String(m_tTime).c_str()
		);
	}
	void PrintHeader() {
		printf(
			"\n%s<<%s %s %s %s %s"
			, Aspect::GetTab()
			, m_sObject.c_str()
			, m_sFunction.c_str()
			, m_sMessage1.c_str()
			, m_sMessage2.c_str()
			, m_sMessage3.c_str()
			//			,String(m_tTime).c_str()
		);
		this->AddTab();
	}
	void PrintFooter() {
		this->RemoveTab();
		printf(
			"\n%s>>%s %s %s %s %s"
			, Aspect::GetTab()
			, m_sObject.c_str()
			, m_sFunction.c_str()
			, m_sMessage1.c_str()
			, m_sMessage2.c_str()
			, m_sMessage3.c_str()
			//			,String(m_tTime).c_str()
		);
	}
};


#if _DEBUG
	#define LOG_HEADER(CLASSNAME, ...) Log(CLASSNAME, __VA_ARGS__).PrintHeader()
	#define LOG_FOOTER(...) Log(__VA_ARGS__).PrintFooter()
	#define LOG_NEWLINE(CLASSNAME, ...) Log(CLASSNAME, __VA_ARGS__).Println()
	#define LOG(CLASSNAME, ...) Log(CLASSNAME, __VA_ARGS__).Print()
//	#define LOG_TIME(...) Log(__VA_ARGS__).PrintTime()
#else
	#define LOG_HEADER(CLASSNAME, ...)
	#define LOG_FOOTER(...)
	#define LOG_NEWLINE(CLASSNAME, ...)
	#define LOG(CLASSNAME, ...)
//	#define LOG_TIME(...) Log(__VA_ARGS__).PrintTime()
#endif