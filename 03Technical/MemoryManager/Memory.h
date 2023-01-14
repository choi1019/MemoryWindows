#pragma once

#include "../../01Base/Aspect/Exception.h"
#include "../../01Base/Object/BaseObject.h"
#include "../../01Base/Memory/IMemory.h"

#include <math.h>

#define SIZE_TOTALMEMORY 10000000
#define SIZE_PAGE 1024

class Page {
public:
	Page* pNext;
};

class PageIndex : public IMemory {
private:
	size_t m_idxPage;
	size_t m_numPages;
	Page* m_pHead;
	PageIndex* m_pNext;

	size_t m_szPage;
	void* m_pMemoryAllocated;


public:
	PageIndex(size_t szPage, size_t szMemoryAllocated, void* pMemoryAllocated)
		: m_szPage(szPage)
		, m_pMemoryAllocated(pMemoryAllocated)

		, m_idxPage(0)
		, m_pNext(nullptr)
	{
		this->m_numPages = szMemoryAllocated / szPage;
		this->m_pHead = reinterpret_cast<Page *>(pMemoryAllocated);
		Page* pCurrentPage = this->m_pHead;
		for (int i = 0; i < m_numPages; i++) {
			pCurrentPage->pNext = pCurrentPage++;
		}
	}
	virtual ~PageIndex() {
	}

};

class PageManager : public IMemory {
private:
	// 
	size_t m_szMemoryAllocated;
	void* m_pMemoryAllocated;
	// page indeces
	PageIndex* m_pHead;

public:
	PageManager(size_t szMemoryAllocated, void *pMemeoryAllocated) {
		this->m_szMemoryAllocated = szMemoryAllocated;
		if (this->m_szMemoryAllocated < SIZE_PAGE) {
			throw Exception(static_cast<unsigned>(IMemory::EException::_eSlotSizeSmall));
		}
		this->m_pMemoryAllocated = pMemeoryAllocated;

		PageIndex* CurrentPageIndex = nullptr;
		this->m_pHead = new PageIndex(SIZE_PAGE, this->m_szMemoryAllocated, this->m_pMemoryAllocated);
	}
};


class Slot {
public:
	Slot* pNext;
};

class SlotIndex : public IMemory {
private:
//	static void* s_pMemoryAllocated;
//	static size_t s_sizeThis;

private:
	unsigned m_idxPage;
	size_t m_szSlot;
	Slot* m_pHead;
	SlotIndex* m_pNext;

public:
	SlotIndex(size_t szSlot)
		: m_idxPage(0)
		, m_szSlot(szSlot)
		, m_pHead(nullptr)
		, m_pNext(nullptr)
	{
	}
	virtual ~SlotIndex() {
	}

	unsigned GetIdxPage() { return this->m_idxPage; }
	size_t GetSzSlot() { return this->m_szSlot; }
	SlotIndex* GetNext() { return this->m_pNext; }
	void SetNext(SlotIndex* pNext) { this->m_pNext = pNext;	}

	Slot* Malloc() { return nullptr; }
	void Free(void* pObject) {}
};

class SlotManager : public IMemory {

private:
	PageManager* m_pPageManager;

	size_t m_szWord;
	size_t m_szWordExponentOf2;
	size_t m_szPage;
	size_t m_szPageExponentOf2;

	size_t m_szAllocated;
	void* m_pMemoryAllocated;
	SlotIndex* m_pHeadSlotIndex;

public:
	SlotManager(size_t szAllocated, void *pMemoryAllocated) {
		// 
		this->m_pPageManager = new PageManager(szAllocated, pMemoryAllocated);

		// WORD size
		this->m_szWord = sizeof(size_t);
		this->m_szWordExponentOf2 = static_cast<size_t>(log2(static_cast<double>(this->m_szWord)));
		// PAGE size
		this->m_szPage = SIZE_PAGE;
		this->m_szPageExponentOf2 = static_cast<size_t>(log2(static_cast<double>(this->m_szPage)));

		this->m_szAllocated = szAllocated;
		this->m_pMemoryAllocated = pMemoryAllocated;

		this->m_pHeadSlotIndex = nullptr;
	}
	virtual ~SlotManager() {
	}

	void* Malloc(size_t szObject) {
		size_t szSlot = szObject;

		// multiple of WORD
		szSlot >>= m_szWordExponentOf2;
		szSlot <<= m_szWordExponentOf2;
		szSlot += m_szWord;

		SlotIndex* pSlotIndex = this->m_pHeadSlotIndex;
		SlotIndex* pPreviousSlotIndex = this->m_pHeadSlotIndex;
		while (pSlotIndex != nullptr) {
			// if slotList is arleady generated
			if (pSlotIndex->GetSzSlot() == szSlot) {
				void *pSlotAllocated = pSlotIndex->Malloc();
				break;
			} 
			pPreviousSlotIndex = pSlotIndex;
			pSlotIndex = pSlotIndex->GetNext();
		}
		// if slotlist of the size is not generated
		if (m_pHeadSlotIndex == nullptr) {
			pSlotIndex = new SlotIndex(szSlot);
			this->m_pHeadSlotIndex = pSlotIndex;
		}
		else {
			pSlotIndex = new SlotIndex(szSlot);
			pSlotIndex->SetNext(pPreviousSlotIndex->GetNext());
			pPreviousSlotIndex->SetNext(pSlotIndex);
		}
		return pSlotIndex->Malloc();
	}
	void Free(void* pObject) {
		size_t idxPage = reinterpret_cast<size_t>(pObject) >> this->m_szPageExponentOf2;
		SlotIndex* pSlotIndex = this->m_pHeadSlotIndex;
		while (pSlotIndex != nullptr) {
			if (pSlotIndex->GetIdxPage() == idxPage) {
				pSlotIndex->Free(pObject);
				break;
			}
			pSlotIndex = pSlotIndex->GetNext();
		}
		throw Exception(static_cast<unsigned>(IMemory::EException::_eNullPtr));
	}
};

class Memory :public IMemory
{

protected:
	// components
	SlotManager* m_pSlotManager;

	// critical section
	virtual void Lock() = 0;
	virtual void UnLock() = 0;
	virtual void* Malloc(size_t sizeAllocate, const char* pcName) {
		void* pObject = this->m_pSlotManager->Malloc(sizeAllocate);
		return pObject;
	}
	virtual void Free(void* pObject) {
		this->m_pSlotManager->Free(pObject);
	}

public:
	// constructors and destructors
	Memory(size_t szMemoryAllocated
		, void* pMemoryAllocated
		, int nClassId = _Memory_Id
		, const char* pClassName = _Memory_Name)
//		, m_szMemoryAllocated(szMemoryAllocated)
//		, m_pMemoryAllocated(pMemoryAllocated)
	{
		this->m_pSlotManager = new SlotManager(szMemoryAllocated, pMemoryAllocated);
	}
	virtual ~Memory() 
	{
	}
	virtual void Initialize() {
	}
	virtual void Finalize() {
	}

//	size_t GetSzAllocated() { return this->m_szMemoryAllocated; }
//	void SetSzAllocated(size_t sizeAllocated) { this->m_szMemoryAllocated = sizeAllocated; }

//	void* GetPAllocated() { return this->m_pMemoryAllocated; }
//	void SetPAllocated(void* pAllocated) { this->m_pMemoryAllocated = pAllocated; }

	// methods
	void* SafeMalloc(size_t szAllocate, const char *pcName = "")
	{
		Lock();
		void* pMemoryAllocated = this->Malloc(szAllocate, pcName);
		UnLock();
		return pMemoryAllocated;
	}
	void SafeFree(void* pObject) {
		Lock();
		this->Free(pObject);
		UnLock();
	}

	// maintenance
	virtual size_t Show(const char* pTitle) { return 0; };
};

#define SHOW_MEMORYSTATIC(MESSAGE) MemoryStatic::s_pMemoryManager->Show(MESSAGE)
#define SHOW_MEMORYEVEN(CLASS, MESSAGE) CLASS::s_pMemory->Show(MESSAGE)
#define SHOW_MEMORYVARIABLE(MESSAGE) BaseObject::s_pMemory->Show(MESSAGE)
