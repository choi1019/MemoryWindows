#pragma once

#include <01Base/typedef.h>
#define _IMemory_Id _GET_CLASS_UID(_ELayer_Base::_eIMemory)
#define _IMemoryName "IMemory"

class IMemory
{
public:
	enum class EException
	{
		_eBegin = _IMemory_Id,
		_eNoMoreSystemMemory,
		_eMemoryAllocatedIsSmallerThanAPage,
		_eNoMorePage,
		_eNoMoreSlot,
		_eSlotSizeSmall,
		_ePageIndexNotFound,
		_eNotSupport,

		_eEnd
	};

	// methods
	virtual void* SafeMalloc(size_t szAllocate, const char* pcName) = 0;
	virtual void SafeFree(void* pPtr) = 0;

	virtual void Show(const char* pTitle) = 0;
};