#ifndef IMEMORY
#define IMEMORY

#include <01Base/typedef.h>
#define _IMEMORY_Id _GET_CLASS_UID(_ELayer_Base::_eIMemory)
#define _IMEMORY_Name "IMemory"

class IMemory
{
public:
	enum class EException
	{
		_eBegin = _IMEMORY_Id,
		_eNoMoreSystemMemory,
		_eMemoryAllocatedIsSmallerThanAPage,
		_eNoMorePage,
		_eNoMoreSlot,
		_eSlotSizeSmall,
		_ePageIndexNotFound,
		_eNotSupport,

		_eEnd
	};

	virtual ~IMemory() = 0;
	virtual void Initialize() = 0;
	virtual void Finalize() = 0;

	// methods
	virtual void* SafeMalloc(size_t szAllocate, const char* pcName) = 0;
	virtual void SafeFree(void* pPtr) = 0;

	virtual void Show(const char* pTitle) = 0;
};

#endif