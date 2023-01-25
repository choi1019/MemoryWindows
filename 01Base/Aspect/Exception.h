#pragma once

#include "../typedef.h"
#define _Exception_Id _GET_CLASS_UID(_ELayer_Base::_eException)
#define _Exception_Name "Exception"

#include "../../01Base/Aspect/Aspect.h"
#include "../../01Base/Memory/IMemory.h"

class Exception : public Aspect
{
protected:
	int m_nType;
	String m_sObject;
	String m_sFunction;
	String m_sMessage;
	time_t m_tTime;

public:
	static IMemory* s_pMemory;
	void* operator new (size_t size) throw() {
		return s_pMemory->SafeMalloc(size, _Exception_Name);
	}
	void operator delete(void* pObject) throw() {
		s_pMemory->SafeFree(pObject);
	}

	Exception(
		int nType = UNDEFINED,
		String sObject = "",
		String sFunction = "",
		String sMessage = "")
		: Aspect(_Exception_Id, _Exception_Name)
		, m_nType(nType)
		, m_sObject(sObject)
		, m_sFunction(sFunction)
		, m_sMessage(sMessage)
	{
		m_tTime = time(0);
	}
	virtual ~Exception() {}

	virtual void Initialize() {	
		Aspect::Initialize();
	}
	virtual void Finalize() {
		Aspect::Finalize();
	}

	virtual void Println() {
		printf(
			"\n>> Execption(%d) << %s::%s => %s\n"
			, m_nType
			, m_sObject.c_str()
			, m_sFunction.c_str()
			, m_sMessage.c_str()
			//			,String(m_tTime).c_str()
		);
	}
	int GetType() { return this->m_nType; }
	String GetSource() { return this->m_sObject; }
	String GetFunction() { return this->m_sFunction; }
	String& GetMessage() { return this->m_sMessage; }
};

