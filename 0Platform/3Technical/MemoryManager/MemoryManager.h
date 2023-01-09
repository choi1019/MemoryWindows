#pragma once

#include "../typedef.h"
#define _MemoryManager_Id _GET_CLASS_UID(_ELayer_Technical::_eMemoryManager)
#define _MemoryManager_Name "PMemoryManager"

#include "../../3Technical/MemoryManager/MemoryStatic.h"

class MemoryManager : public MemoryStatic
{
private:
	Vector<Memory*> m_vMemory;

public:
	MemoryManager(
		size_t szAllocated,
		int nClassd = _MemoryManager_Id,
		const char* pClassName = _MemoryManager_Name)
		: MemoryStatic(szAllocated, nClassd, pClassName)
	{
	}
	virtual ~MemoryManager() 
	{
	}

	virtual void InitializeMemory() {
		MemoryStatic::InitializeMemory();
	}
//	virtual void InitializeMemory(size_t szMemoryBase, size_t countException,	size_t countLog, size_t countEvent) = 0;

	virtual void FinalizeMemory() {
		MemoryStatic::FinalizeMemory();
	}

	Vector<Memory*>& GetVMemory() { return this->m_vMemory; }
	void Add(Memory* pIMemory) { this->m_vMemory.Add(pIMemory); }
};


