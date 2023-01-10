#pragma once
#pragma warning(disable: 4291; disable: 4297)

#include "../typedef.h"
#define _MemoryDynamic_Id _GET_CLASS_UID(_ELayer_Technical::_eMemoryDynamic)
#define _MemoryDynamic_Name "MemoryDynamic"

#include "../../3Technical/MemoryManager/Memory.h"
#undef GetClassName

class MemoryDynamic : public Memory
{
public:
	MemoryDynamic(
		size_t szAllocated,
		int nClassId = _MemoryDynamic_Id,
		const char* pClassName = _MemoryDynamic_Name)
		: Memory(nClassId, pClassName)
	{
		this->SetSzThis(*(size_t*)((long long)this - sizeof(size_t)));
		this->SetSzAllocated(szAllocated);
		this->SetPAllocated(nullptr);
	}
	virtual ~MemoryDynamic() {
	}

protected:
	virtual void InitializeMemory() {
		Memory::InitializeMemory();
		this->SetPAllocated(
			Memory::s_pMemoryManager->SafeMalloc(this->GetSzAllocated(), this->GetClassName()));
	}

	virtual void FinalizeMemory() {
		Memory::FinalizeMemory();
		Memory::s_pMemoryManager->SafeFree(this->GetPAllocated());
	}
};

