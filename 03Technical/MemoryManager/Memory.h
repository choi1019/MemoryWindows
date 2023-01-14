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
	size_t m_index;
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

		, m_pNext(nullptr)
	{
		this->m_numPages = szMemoryAllocated / szPage;
		this->m_pHead = reinterpret_cast<Page*>(pMemoryAllocated);
		this->m_index = (size_t)m_pHead - szPage;

		Page* pPage = this->m_pHead;
		Page* pPreviousPage = this->m_pHead;
		while (m_szMemoryAllocated >= ((size_t)pPage-(size_t)m_pHead+SIZE_PAGE)) {
			pPreviousPage = pPage;
			pPage->pNext = (Page*)((size_t)pPage + m_szPage);
			pPage = pPage->pNext;
		}
		pPreviousPage->pNext = nullptr;
	}

	virtual ~PageIndex() {}

	size_t GetIndex() { return this->m_index; }
	size_t GetNumPages() { return this->m_numPages; }
	PageIndex* GetPNext() { return this->m_pNext; }

	Page* Malloc(size_t numPages) {
		Page* pPage = this->m_pHead;
		for (int i = 0; i < numPages; i++) {
			this->m_pHead = this->m_pHead->pNext;
		}
		return pPage;
	}
	void Free(Page *pPageFree) {
		if (pPageFree < m_pHead) {
			pPageFree->pNext = m_pHead;
			m_pHead = pPageFree;
		}
		else {
			Page* pPage = this->m_pHead;
			Page* pPreviousPage = this->m_pHead;
			while (pPage != nullptr) {
				if (pPageFree > pPage) {
					break;
				}
				pPreviousPage = pPage;
				pPage = pPage->pNext;
			}
			pPageFree->pNext = pPage;
			pPreviousPage->pNext = pPageFree;
		}
	}

	void Show(const char* pTitle) {
		Page* pPage = m_pHead;
		Page* pPreviousPage = m_pHead;
		LOG_HEADER("PageIndex::show", String((size_t)m_index), String((size_t)m_numPages));
		while (pPage != nullptr) {
			size_t numPage = ((size_t)pPage - (size_t)m_pMemoryAllocated) / SIZE_PAGE;
			LOG(String(numPage), String(" "), String((size_t)pPage));
			pPreviousPage = pPage;
			pPage = pPage->pNext;
		}
		LOG_FOOTER("PageIndex");
	}

};

class PageManager : public IMemory {
private:
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

	Page* Malloc(size_t szObject) {
		if (this->m_pHead == nullptr) {
			throw Exception((unsigned)IMemory::EException::_eNoMoreSlot);
		}
		size_t numPages = szObject / SIZE_PAGE + 1;
		PageIndex* pPageIndex = this->m_pHead;
		PageIndex* pPreviousPageIndex = this->m_pHead;
		while (pPageIndex != nullptr) {
			if (pPageIndex->GetNumPages() >= numPages) {
				Page* pPage = pPageIndex->Malloc(numPages);
				if (pPage != nullptr) {
					return pPage;
				}
			}
			pPreviousPageIndex = pPageIndex;
			pPageIndex = pPageIndex->GetPNext();
		}
		throw Exception((unsigned)IMemory::EException::_eNoMoreSlot);
	}

	void Free(Page* pPageFree) {
		if (m_pHead == nullptr) {
			/* ////////////////////////////////////////
			this->m_pHead = new PageIndex(SIZE_PAGE, this->m_szMemoryAllocated, this->m_pMemoryAllocated);
			pPageFree->pNext = m_pHead;
			m_pHead = pPageFree;
			*/
		}
		else {
			PageIndex* pPageIndex = this->m_pHead;
			PageIndex* pPreviousPageIndex = this->m_pHead;
			while (pPageIndex != nullptr) {
				if ((size_t)pPageFree > pPageIndex->GetIndex()) {
					break;
				}
				pPreviousPageIndex = pPageIndex;
				pPageIndex = pPageIndex->GetPNext();
			}
			pPreviousPageIndex->Free(pPageFree);
		}
	}

	void Show(const char* pTitle) {
		PageIndex* pCurrentPageIndex = m_pHead;
		PageIndex* pPreviousPageIndex = m_pHead;
		LOG_HEADER("PageManager::Show", 
			String(m_szMemoryAllocated), String(SIZE_PAGE), String((unsigned)(m_szMemoryAllocated)/SIZE_PAGE));
		while (pCurrentPageIndex != nullptr) {
			pCurrentPageIndex->Show("");
			pPreviousPageIndex = pCurrentPageIndex;
			pCurrentPageIndex = pCurrentPageIndex->GetPNext();
		}
		LOG_FOOTER("PageManager");
	}
};


class Slot {
public:
	Slot* pNext;
};

class SlotIndex : public IMemory {

private:
	static size_t s_szCount;

	size_t m_index;

	size_t m_szSlot;
	Page* m_pPage;

	Slot* m_pHead;

	SlotIndex* m_pNext;

public:
	SlotIndex(size_t szSlot, Page* pPage)
		: m_pNext(nullptr)
	{
		this->m_szSlot = szSlot;
		this->m_pPage = pPage;

		this->m_index = (size_t)pPage;
		this->m_pHead = (Slot*)pPage;
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

	size_t GetIndex() { return this->m_index; }
	size_t GetSzSlot() { return this->m_szSlot; }
	SlotIndex* GetNext() { return this->m_pNext; }
	void SetNext(SlotIndex* pNext) { this->m_pNext = pNext;	}

	Slot* Malloc() { 
		if (this->m_pHead == nullptr) {
			// add more page ///////////////////////////
			throw Exception((unsigned)IMemory::EException::_eNoMoreSlot);
		}
		Slot* pSlot = this->m_pHead;
		this->m_pHead = this->m_pHead->pNext;
		return pSlot;
	}
	void Free(void* pObject) {
		Slot* pSlotFree = (Slot*)pObject;
		pSlotFree->pNext = m_pHead;
		m_pHead = pSlotFree;
	}

	// maintenance
	virtual void Show(const char* pTitle) {

		LOG_HEADER("SlotIndex::Show", String(m_szSlot), String((unsigned)(SIZE_PAGE/ m_szSlot)), String((size_t)m_pPage));
		Slot* pSlot = this->m_pHead;
		while (pSlot != nullptr) {
			LOG("Slot-", (size_t)pSlot);
			pSlot = pSlot->pNext;
		}
		LOG_FOOTER("SlotIndex");
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
	SlotIndex* m_pHead;

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

		this->m_pHead = nullptr;
	}
	virtual ~SlotManager() {
	}

	void* Malloc(size_t szObject) {
		size_t szSlot = szObject;

		// multiple of WORD
		szSlot >>= m_szWordExponentOf2;
		szSlot <<= m_szWordExponentOf2;
		if (szSlot < szObject) {
			szSlot += m_szWord;
		}

		SlotIndex* pSlotIndex = this->m_pHead;
		if (pSlotIndex == nullptr) {
			Page* newPage = this->m_pPageManager->Malloc(szObject);
			pSlotIndex = new SlotIndex(szSlot, newPage);
			this->m_pHead = pSlotIndex;
			return pSlotIndex->Malloc();
		}
		SlotIndex* pPreviousSlotIndex = this->m_pHead;
		while (pSlotIndex != nullptr) {
			// if slotList is arleady generated
			if (pSlotIndex->GetSzSlot() == szSlot) {
				return pSlotIndex->Malloc();
			} 
			// insert new SlotIndex
			else if (pSlotIndex->GetSzSlot() > szSlot) {
				break;
			}
			pPreviousSlotIndex = pSlotIndex;
			pSlotIndex = pSlotIndex->GetNext();
		}

		// if slotlist of the size is not generated
		Page* newPage = this->m_pPageManager->Malloc(szObject);
		pSlotIndex = new SlotIndex(szSlot, newPage);
		pSlotIndex->SetNext(pPreviousSlotIndex->GetNext());
		pPreviousSlotIndex->SetNext(pSlotIndex);
		return pSlotIndex->Malloc();
	}

	void Free(void* pObject) {
		Slot* pSlotFree = (Slot*)pObject;
		SlotIndex* pSlotIndex = this->m_pHead;
		SlotIndex* pPreviousSlotIndex = this->m_pHead;
		while (pSlotIndex != nullptr) {
			// if slotList is arleady generated
			if (pSlotIndex->GetSzSlot() == szSlot) {
				return pSlotIndex->Malloc();
			}
			// insert new SlotIndex
			else if (pSlotIndex->GetSzSlot() > szSlot) {
				break;
			}
			pPreviousSlotIndex = pSlotIndex;
			pSlotIndex = pSlotIndex->GetNext();
		}
		throw Exception(static_cast<unsigned>(IMemory::EException::_eNullPtr));
		*/
	}

	// maintenance
	virtual void Show(const char* pTitle) {
		this->m_pPageManager->Show("");

		LOG_HEADER("SlotManager::Show");
		SlotIndex* pSlotIndex = this->m_pHead;
		while (pSlotIndex != nullptr) {
			pSlotIndex->Show("");
			pSlotIndex = pSlotIndex->GetNext();
		}
		LOG_FOOTER("SlotManager");
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
		LOG_HEADER("Memory::Show");
		this->m_pSlotManager->Show("");
		LOG_FOOTER("Memory");
	};
};
