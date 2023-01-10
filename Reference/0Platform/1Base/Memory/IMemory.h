#pragma once

#include "../typedef.h"
#define _Memory_Id _GET_CLASS_UID(_ELayer_Base::_eMemory)
#define _Memory_Name "Memory"

class IMemory
{
public:
	enum class EError
	{
		_eBegin = _Memory_Id,
		_eOutOfMemory,
		_eNoMoreSlot,
		_eSlotCountZero,
		_eSlotSizeSmall,
		_eNullPtr,
		_eNotSupported,
		_eEnd
	};

	virtual ~IMemory() {};
	virtual void InitializeMemory() = 0;
	virtual void FinalizeMemory() = 0;

	// methods
	virtual void* SafeMalloc(size_t szAllocate, const char* pcName) = 0;
	virtual void SafeFree(void* pPtr) = 0;

	virtual size_t Show(const char* pTitle) = 0;
};
