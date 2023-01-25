#pragma once

#include "../typedef.h"
#define _Memory_Id _GET_CLASS_UID(_ELayer_Base::_eMemory)
#define _Memory_Name "Memory"

class IMemory
{
public:
	enum class EException
	{
		_eBegin = _Memory_Id,
		_eMemoryAllocatedIsSmallerThanAPage,
		_eNoMorePage,
		_eOutOfMemory,
		_eNoMoreSlot,
		_eSlotCountZero,
		_eSlotSizeSmall,
		_eNullPtr,
		_eNotSupported,
		_eFree,
		_eEnd
	};


	virtual ~IMemory() {};
	virtual void Initialize() {}
	virtual void Finalize() {}

	// methods
	virtual void* SafeMalloc(size_t szAllocate, const char* pcName) { return nullptr;  }
	virtual void SafeFree(void* pPtr) {}

	virtual void Show(const char* pTitle) { }
};

