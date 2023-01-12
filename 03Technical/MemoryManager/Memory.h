#pragma once

#include "../../01Base/Aspect/Exception.h"
#include "../../01Base/Object/BaseObject.h"
#include "../../01Base/Memory/IMemory.h"

#define PAGESIZE 1024
class Page {
public:
	Page* pNext;
};

class PageList {
private:
	char* pMemoryAllocated;
	size_t numPagesTotal;

	Page* pHeadFree;
	size_t numPagesFree;

	Page* pHeadAllocated;

	size_t GetIndex(Page* pPage) {
		size_t index = (reinterpret_cast<size_t>(pPage) - reinterpret_cast<size_t>(this->pMemoryAllocated)) / PAGESIZE;
	}

public:
	PageList(size_t szAllocated, char* pMemoryAllocated) {
		this->pMemoryAllocated = pMemoryAllocated;
		this->numPagesTotal = szAllocated / PAGESIZE;
		if (numPagesTotal < 1) {
			throw Exception(static_cast<int>(IMemory::EException::_eOutOfMemory));
		}

		this->pHeadFree = reinterpret_cast<Page*>(this->pMemoryAllocated);
		Page* pPage;
		for (pPage = this->pHeadFree; pPage + PAGESIZE <= this->pHeadFree + szAllocated; pPage += PAGESIZE) {
			pPage->pNext = pPage + PAGESIZE;
		}
		pPage->pNext = nullptr;

		this->pHeadAllocated = nullptr;
		this->numPagesFree = this->numPagesTotal;
	}

	Page* Allocate() {
		if (this->pHeadFree == nullptr) {
			throw Exception(static_cast<int>(IMemory::EException::_eOutOfMemory));
		}
		Page* pNewPage = this->pHeadFree;
		this->pHeadFree = this->pHeadFree->pNext;
		this->numPagesFree++;
	}
	void Delocate(Page* pPage) {
		pPage->pNext = this->pHeadFree;
		this->pHeadFree = pPage;
		this->numPagesFree--;
	}
};

class Memory :public IMemory, public BaseObject
{
public:
	static char* s_pMemoryAllocated;
	static size_t s_sizeThis;

	void* operator new(size_t szThis, char* pMemoryAllocated) {
		s_sizeThis = szThis;
		s_pMemoryAllocated = pMemoryAllocated;
		return s_pMemoryAllocated;
	}
	void operator delete(void* pObject) {
		s_pMemoryAllocated = nullptr;
	}
	//  matching operator - new(size_t szThis, const char* pcName="")
	void operator delete(void* pObject, char* pMemoryAllocated) {
		s_pMemoryAllocated = nullptr;
	}

private:
	// attributes
	size_t m_szAllocated;
	void* m_pAllocated;

	virtual void* Malloc(size_t szAllocate, const char* pcName = "") = 0;
	virtual void Free(void* ptr) = 0;
public:
	// constructors and destructors
	Memory(int nClassId = _Memory_Id,
		const char* pClassName = _Memory_Name)
		: BaseObject(nClassId, pClassName)
		, m_szAllocated(0)
		, m_pAllocated(nullptr)
	{
	}
	virtual ~Memory() 
	{
	}
	virtual void Initialize() {
	}
	virtual void Finalize() {
	}	

	// critical section
	virtual void Lock() = 0;
	virtual void UnLock() = 0;

	size_t GetSzAllocated() { return this->m_szAllocated; }
	void SetSzAllocated(size_t szAllocated) { this->m_szAllocated = szAllocated; }

	void* GetPAllocated() { return this->m_pAllocated; }
	void SetPAllocated(void* pAllocated) { this->m_pAllocated = pAllocated; }

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
