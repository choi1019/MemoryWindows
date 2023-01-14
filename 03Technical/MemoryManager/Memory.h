#pragma once

#include "../../01Base/Aspect/Exception.h"
#include "../../01Base/Aspect/Log.h"

#include "../../01Base/Object/BaseObject.h"
#include "../../01Base/Memory/IMemory.h"

#include <math.h>

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
	size_t m_szMemoryAllocated;
	void* m_pMemoryAllocated;


public:
	PageIndex(size_t szPage, size_t szMemoryAllocated, void* pMemoryAllocated)
		: m_szPage(szPage)
		, m_szMemoryAllocated(szMemoryAllocated)
		, m_pMemoryAllocated(pMemoryAllocated)

		, m_idxPage(0)
		, m_pNext(nullptr)
	{
		this->m_numPages = szMemoryAllocated / szPage;
		this->m_pHead = reinterpret_cast<Page*>(pMemoryAllocated);
		Page* pCurrentPage = this->m_pHead;
		Page* pPreviousPage = this->m_pHead;
		while (m_szMemoryAllocated > (size_t)pCurrentPage - (size_t)m_pHead) {
			size_t numPage = ((size_t)pCurrentPage - (size_t)m_pMemoryAllocated) / SIZE_PAGE;
			pPreviousPage = pCurrentPage;
			pCurrentPage->pNext = (Page*)((size_t)pCurrentPage + m_szPage);
			pCurrentPage = pCurrentPage->pNext;
		}
		pPreviousPage->pNext = nullptr;
	}
	virtual ~PageIndex() {
	}

	size_t GetNumPages() { return this->m_numPages; }
	PageIndex* GetPNext() { return this->m_pNext; }

	Page* Malloc(size_t numPages) {
		Page* pPage = this->m_pHead;
		for (int i = 0; i < numPages; i++) {
			this->m_pHead = this->m_pHead->pNext;
		}
		return pPage;
	}
	void Free() {

	}
	void Show(const char* pTitle) {
		Page* pCurrentPage = m_pHead;
		Page* pPreviousPage = m_pHead;
		while (pCurrentPage != nullptr) {
			LOG_HEADER("PageIndex", "Show");
			size_t numPage = ((size_t)pCurrentPage - (size_t)m_pMemoryAllocated) / SIZE_PAGE;
			LOG(String(numPage));
			LOG_FOOTER("PageIndex");
			pPreviousPage = pCurrentPage;
			pCurrentPage = pCurrentPage->pNext;
		}
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

	Page* Malloc(size_t numPages) {
		if (this->m_pHead == nullptr) {
			throw Exception((unsigned)IMemory::EException::_eNoMoreSlot);
		}
		
		PageIndex* currentPageIndex = this->m_pHead;
		PageIndex* previousPageIndex = this->m_pHead;
		while (currentPageIndex != nullptr) {
			if (currentPageIndex->GetNumPages() > numPages) {
				Page* pPage = currentPageIndex->Malloc(numPages);
				if (pPage != nullptr) {
					return pPage;
				}
			}
			previousPageIndex = currentPageIndex;
			currentPageIndex = currentPageIndex->GetPNext();
		}
		throw Exception((unsigned)IMemory::EException::_eNoMoreSlot);
	}

	void Free(void* pObject) {
	}

	void Show(const char* pTitle) {
		PageIndex* pCurrentPageIndex = m_pHead;
		PageIndex* pPreviousPageIndex = m_pHead;
		while (pCurrentPageIndex != nullptr) {
			LOG_HEADER("PageManager", String((size_t)pCurrentPageIndex));
			pCurrentPageIndex->Show("");
			LOG_FOOTER("PageManager");
			pPreviousPageIndex = pCurrentPageIndex;
			pCurrentPageIndex = pCurrentPageIndex->GetPNext();
		}
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
	SlotIndex(size_t szSlot, Page* pPage)
		: m_szSlot(szSlot)
		, m_pHead((Slot*)pPage)

		, m_idxPage(0)
		, m_pNext(nullptr)
	{
		size_t numSlots = SIZE_PAGE / szSlot;
		Slot* pSlot = this->m_pHead;
		Slot* pPreviousSlot = this->m_pHead;
		for (int i = 0; i < numSlots; i++) {
			pSlot->pNext = (Slot*)((size_t)pSlot + m_szSlot);
			pPreviousSlot = pSlot;
			pSlot = pSlot->pNext;
		}
		pPreviousSlot->pNext = nullptr;
	}
	virtual ~SlotIndex() {
	}

	unsigned GetIdxPage() { return this->m_idxPage; }
	size_t GetSzSlot() { return this->m_szSlot; }
	SlotIndex* GetNext() { return this->m_pNext; }
	void SetNext(SlotIndex* pNext) { this->m_pNext = pNext;	}

	Slot* Malloc() { 
		if (this->m_pHead == nullptr) {
			// add more page
			throw Exception((unsigned)IMemory::EException::_eNoMoreSlot);
		}
		Slot* pSlot = this->m_pHead;
		this->m_pHead = this->m_pHead->pNext;
		return pSlot;
	}
	void Free(void* pObject) {}

	// maintenance
	virtual void Show(const char* pTitle) {

	};
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
			Page* newPage = this->m_pPageManager->Malloc(szObject/SIZE_PAGE + 1);
			pSlotIndex = new SlotIndex(szSlot, newPage);
			this->m_pHeadSlotIndex = pSlotIndex;
		}
		else {
			Page* newPage = this->m_pPageManager->Malloc(szObject / SIZE_PAGE + 1);
			pSlotIndex = new SlotIndex(szSlot, newPage);
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

	// maintenance
	virtual void Show(const char* pTitle) {
		this->m_pPageManager->Show("");
	};
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
	virtual void Show(const char* pTitle) { 
		this->m_pSlotManager->Show("");
	};
};
