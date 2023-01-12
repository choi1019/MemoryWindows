#pragma once

#include "../../01Base/Memory/IMemory.h"
#include "../../01Base/Object/BaseObject.h"
//#include "../../2Platform/Component/ComponentPart.h"

class Memory :public IMemory, public BaseObject
{
public:
	static IMemory* s_pMemoryManager;

	void* operator new(size_t szThis, const char* pcName="") {
		void* pAllocated = s_pMemoryManager->SafeMalloc(szThis, pcName);
		return pAllocated;
	}
	void operator delete(void* pObject) {
		s_pMemoryManager->SafeFree(pObject);
	}
	//  matching operator - new(size_t szThis, const char* pcName="")
	void operator delete(void* pObject, const char* pcName) {
		s_pMemoryManager->SafeFree(pObject);
	}

private:
	// attributes
	size_t m_szAllocated;
	void* m_pAllocated;

	virtual void* Malloc(size_t szAllocate, const char* pcName) = 0;
	virtual void Free(void* ptr) = 0;

public:
	// critical section
	virtual void Lock() = 0;
	virtual void UnLock() = 0;

	size_t GetSzAllocated() { return this->m_szAllocated; }
	void SetSzAllocated(size_t szAllocated) { this->m_szAllocated = szAllocated; }

	void* GetPAllocated() { return this->m_pAllocated; }
	void SetPAllocated(void* pAllocated) { this->m_pAllocated = pAllocated; }

public:
	// constructors and destructors
	Memory(int nClassId = _Memory_Id,
		const char* pClassName = _Memory_Name)
		: BaseObject(nClassId, pClassName)
//		: ComponentPart(nClassId, pClassName)
		, m_szAllocated(0)
		, m_pAllocated(nullptr)
	{
	}
	virtual ~Memory() 
	{
	}
	virtual void Initialize() {
//		ComponentPart::Initialize();

	}
	virtual void Finalize() {
//		ComponentPart::Finalize();
	}	

	// methods
	void* SafeMalloc(size_t szAllocate, const char *pcName)
	{
		Lock();
		void* pMemoryAllocated = this->Malloc(szAllocate, pcName);
		UnLock();
		return pMemoryAllocated;
	}
	void SafeFree(void* pPtr) {
		Lock();
		this->Free(pPtr);
		UnLock();
	}

	virtual void SetId(void* pObject) {}

	// maintenance
	virtual size_t Show(const char* pTitle) = 0;
};

#define SHOW_MEMORYSTATIC(MESSAGE) MemoryStatic::s_pMemoryManager->Show(MESSAGE)
#define SHOW_MEMORYEVEN(CLASS, MESSAGE) CLASS::s_pMemory->Show(MESSAGE)
#define SHOW_MEMORYVARIABLE(MESSAGE) BaseObject::s_pMemory->Show(MESSAGE)
