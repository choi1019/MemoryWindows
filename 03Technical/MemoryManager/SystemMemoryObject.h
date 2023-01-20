#pragma once
#include "../../01Base/Object/BaseObject.h"

class SystemMemoryObject : public BaseObject {
public:
	// system memory allocated
	static size_t s_szSystemMemoryAllocated;
	static void* s_pSystemMemoryAllocated;
	static void* s_pCurrentSystemMemoryAllocated;

	static void Allocate(size_t szSystemMemory, void* pSystemMemory) {
		s_szSystemMemoryAllocated = szSystemMemory;
		s_pSystemMemoryAllocated = pSystemMemory;
		s_pCurrentSystemMemoryAllocated = pSystemMemory;
	}

	void* operator new(size_t szThis, const char *sMessage) {
		s_szSystemMemoryAllocated += szThis;
		void* pCurrentSystemMemoryAllocated = s_pCurrentSystemMemoryAllocated;
		s_pCurrentSystemMemoryAllocated = reinterpret_cast<void*>(reinterpret_cast<size_t>(s_pCurrentSystemMemoryAllocated) + szThis);
		return pCurrentSystemMemoryAllocated;
	}
	void operator delete(void* pObject) {
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