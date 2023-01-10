#pragma once

#include "../typedef.h"
#define _PMemoryVariable_Id _GET_CLASS_UID(_EPPlatform::_ePMemoryVariable)
#define _PMemoryVariable_Name "PMemoryVariable"

#include "../../3Technical/MemoryManager/MemoryVariable.h"
#include <Windows.h>

class PMemoryVariable : public MemoryVariable {
private:
	CRITICAL_SECTION CriticalSection;

protected:
	virtual void Lock() {
		EnterCriticalSection(&CriticalSection);
	}
	virtual void UnLock() {
		LeaveCriticalSection(&CriticalSection);
	}

public:
	PMemoryVariable(
		size_t szAllocated,
		int nClassId = _PMemoryVariable_Id,
		const char* pcClassName = _PMemoryVariable_Name)
		: MemoryVariable(szAllocated, nClassId, pcClassName)
	{
		InitializeCriticalSection(&CriticalSection);
	}
	virtual ~PMemoryVariable() {
		DeleteCriticalSection(&CriticalSection);
	}

	virtual void Initialize() {
		MemoryVariable::Initialize();
	}
	virtual void Finalize() {
		MemoryVariable::Finalize();
	}
};