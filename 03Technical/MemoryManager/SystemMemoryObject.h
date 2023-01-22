#pragma once
#include "../../01Base/Object/BaseObject.h"
#include "../../01Base/Aspect/Log.h"

class SystemMemoryObject : public BaseObject {
public:
	// system memory allocated
	static size_t s_szSystemMemoryAllocated;
	static void* s_pSystemMemoryAllocated;
	static void* s_pCurrentSystemMemoryAllocated;

	void* operator new(size_t szThis, const char *sMessage) {
		LOG_NEWLINE("@new SystemMemoryObject(sMessage, szThis)", sMessage, szThis);

		s_szSystemMemoryAllocated -= szThis;
		void* pCurrentSystemMemoryAllocated = s_pCurrentSystemMemoryAllocated;
		s_pCurrentSystemMemoryAllocated = (void*)((size_t)s_pCurrentSystemMemoryAllocated + szThis);
		return pCurrentSystemMemoryAllocated;
	}
	void operator delete(void* pObject) {
		LOG_NEWLINE("@delete SystemMemoryObject(pObject)", (size_t)pObject);
	}
	void operator delete(void* pObject, const char* sMessage) {
	//	s_szSystemMemoryAllocated = 0;
	//		s_pSystemMemoryAllocated = nullptr;
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