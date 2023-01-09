#pragma once

#include "../typedef.h"
#define _Log_Id _GET_CLASS_UID(_ELayer_Base::_eLog)
#define _Log_Name "Log"

#include "../../1Base/Aspect/Aspect.h"
#include "../../1Base/Memory/IMemory.h"

class Log : public Aspect
{
protected:
	String m_sObject;
	String m_sFunction;
	String m_sMessage1;
	String m_sMessage2;
	time_t m_tTime;

public:
	static IMemory* s_pMemory;
	void* operator new (size_t szSize) {
		return s_pMemory->SafeMalloc(szSize, "Log::s_pMemory");
	}
	void operator delete(void* pObject) {
		s_pMemory->SafeFree(pObject);
	}

	Log( String sObject = "",
		 String sFunction = "",
		 String sMessage1 = "",
		 String sMessage2 = "",
		unsigned classId = _Log_Id,
		const char* pClassName = _Log_Name)
		: Aspect(classId, pClassName)
		, m_sObject(sObject)
		, m_sFunction(sFunction)
		, m_sMessage1(sMessage1)
		, m_sMessage2(sMessage2)
	{
		m_tTime = time(0);

	}
	virtual ~Log() {}

	virtual void Initialize() {
		Aspect::Initialize();
	}
	virtual void Finalize() {
		Aspect::Finalize();
	}
	void Println() {
		printf(
			"%s%s %s %s %s\n"
			, this->GetTab()
			,m_sObject.c_str()
			,m_sFunction.c_str()
			, m_sMessage1.c_str()
			, m_sMessage2.c_str()
//			,String(m_tTime).c_str()
		);
	}
	void PrintHeader() {
		printf(
			"\n%s<<%s %s %s %s\n"
			, this->GetTab()
			, m_sObject.c_str()
			, m_sFunction.c_str()
			, m_sMessage1.c_str()
			, m_sMessage2.c_str()
			//			,String(m_tTime).c_str()
		);
		this->AddTab();
	}
	void PrintFooter() {
		this->RemoveTab();
		printf(
			"%s%s>> %s %s %s\n"
			, this->GetTab()
			, m_sObject.c_str()
			, m_sFunction.c_str()
			, m_sMessage1.c_str()
			, m_sMessage2.c_str()
			//			,String(m_tTime).c_str()
		);
	}

	String GetSource() { return this->m_sObject; }
	String GetFunction() { return this->m_sFunction; }
	String GetMessage1() { return this->m_sMessage1; }
	String GetMessage2() { return this->m_sMessage2; }
//	String GetTime() { return String(m_tTime); }
};

#define LOG_HEADER(CLASSNAME, ...) Log(CLASSNAME, __VA_ARGS__).PrintHeader()
#define LOG_FOOTER(...) Log(__VA_ARGS__).PrintFooter()
#define LOG(CLASSNAME, ...) Log(CLASSNAME, __VA_ARGS__).Println()
#define LOG_SEPARATOR() Log().PrintSeparator()
#define LOG_NEXTLINE() Log().PrintNextLine()

