#pragma once
#include "../../01Base/Object/BaseObject.h"
#include "../../01Base/Aspect/Log.h"
#include "../../01Base/Aspect/Exception.h"

class SystemMemoryObject : public BaseObject {
public:
	// system memory allocated
	static size_t s_szSystemMemoryAllocated;
	static void* s_pSystemMemoryAllocated;
	static void* s_pCurrentSystemMemoryAllocated;

	void* operator new(size_t szThis, const char *sMessage) {
		if (s_szSystemMemoryAllocated < szThis) {
			throw Exception((unsigned)IMemory::EException::_eNoMoreSystemMemory, sMessage, "new", "_eNoMoreSystemMemory");
		}
		s_szSystemMemoryAllocated -= szThis;
		void* pCurrentSystemMemoryAllocated = s_pCurrentSystemMemoryAllocated;
		s_pCurrentSystemMemoryAllocated = (void*)((size_t)s_pCurrentSystemMemoryAllocated + szThis);
		LOG_NEWLINE("@new ", sMessage, "(szThis, pAllocated)", szThis, (size_t)pCurrentSystemMemoryAllocated);
		return pCurrentSystemMemoryAllocated;
	}
	void* operator new[](size_t szThis, const char* sMessage) {
		return SystemMemoryObject::operator new(szThis, sMessage);
	}
	void operator delete(void* pObject) {
		LOG_NEWLINE("@delete SystemMemoryObject(pObject)", (size_t)pObject);
	}
	void operator delete[](void* pObject) {
		LOG_NEWLINE("@delete[] SystemMemoryObject(pObject)", (size_t)pObject);
	}

	void operator delete(void* pObject, const char* sMessage) {
	}
	void operator delete[](void* pObject, const char* sMessage) {
	}

public:
	SystemMemoryObject() {
	}
	virtual ~SystemMemoryObject() {
	}
	virtual void Initialize() {
	}
	virtual void Finalize() {
	}
};