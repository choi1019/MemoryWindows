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
		_eOutOfMemory,
		_eNoMoreSlot,
		_eSlotCountZero,
		_eSlotSizeSmall,
		_eNullPtr,
		_eNotSupported,
		_eFree,
		_eEnd
	};

	// system memory allocated
	static size_t s_szSystemMemoryAllocated;
	static void* s_pSystemMemoryAllocated;

	void* operator new(size_t szThis) {
		s_szSystemMemoryAllocated -= szThis;
		void* pMemoryAllocated = s_pSystemMemoryAllocated;
		s_pSystemMemoryAllocated = reinterpret_cast<void*>(reinterpret_cast<size_t>(pMemoryAllocated) + szThis);
		return pMemoryAllocated;
	}
	void operator delete(void* pObject) {
		s_szSystemMemoryAllocated = 0;
		s_pSystemMemoryAllocated = nullptr;
	}

	virtual ~IMemory() {};
	virtual void Initialize() {}
	virtual void Finalize() {}

	// methods
	virtual void* SafeMalloc(size_t szAllocate, const char* pcName) { return nullptr;  }
	virtual void SafeFree(void* pPtr) {}

	virtual void Show(const char* pTitle) { }
};
