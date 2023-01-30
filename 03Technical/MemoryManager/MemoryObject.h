#pragma once

#include <03Technical/typedef.h>
#define _MemoryObject_Id _GET_CLASS_UID(_ELayer_Technical::_eMemoryObject)
#define _MemoryObject_Name "MemoryObject"

#include <01Base/Object/BaseObject.h>

class MemoryObject : public BaseObject {
public:
	// system memory allocated
	static size_t s_szSystemMemoryAllocated;
	static void* s_pSystemMemoryAllocated;
	static void* s_pCurrentSystemMemoryAllocated;

	void* operator new(size_t szThis, const char *sMessage);
	void* operator new[](size_t szThis, const char* sMessage);
	void operator delete(void* pObject);
	void operator delete[](void* pObject);

	void operator delete(void* pObject, const char* sMessage);
	void operator delete[](void* pObject, const char* sMessage);

public:
	MemoryObject(
		int nClassId = _MemoryObject_Id,
		const char* pClassName = _MemoryObject_Name)
		: BaseObject(nClassId, pClassName)
	{
	}
	virtual ~MemoryObject() {
	}
	virtual void Initialize() {
	}
	virtual void Finalize() {
	}
};