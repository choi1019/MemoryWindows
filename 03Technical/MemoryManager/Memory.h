#pragma once

#include "../../01Base/Aspect/Exception.h"
#include "../../01Base/Aspect/Log.h"

#include "../../01Base/Object/BaseObject.h"
#include "../../01Base/Memory/IMemory.h"

#include <math.h>

class Page {
public:
	Page* pNext;
};

class PageIndex : public IMemory {
private:
	Page* m_pPage;
	PageIndex* m_pNext;
public:
	PageIndex(Page *pPage)
		: m_pPage(pPage)
		, m_pNext(nullptr)
	{
	}
	virtual ~PageIndex() {}

	Page* GetPPage() { return this->m_pPage; }
	void SetPPage(Page* pPage) { this->m_pPage = pPage; }
	PageIndex* GetPNext() { return this->m_pNext; }
	void SetPNext(PageIndex* pNext) { this->m_pNext = pNext; }

	void Show(const char* pTitle) {
		LOG_HEADER("PageIndex::show - Free", String((size_t)m_pPage), String((size_t)m_pNext));
		LOG_FOOTER("PageIndex");
	}

};

class PageManager : public IMemory {
private:
	size_t m_szPage;
	size_t m_numPages;
	size_t m_szMemoryAllocated;
	void* m_pMemoryAllocated;

	// pageIndex
	PageIndex* m_pHead;

public:
	PageManager(size_t szPage, size_t szMemoryAllocated, void *pMemeoryAllocated) {
		this->m_szPage = szPage;
		this->m_numPages = szMemoryAllocated / szPage;

		this->m_szMemoryAllocated = szMemoryAllocated;
		if (this->m_szMemoryAllocated < m_szPage) {
			throw Exception(static_cast<unsigned>(IMemory::EException::_eMemoryAllocatedIsSmallerThanAPage));
		}

		this->m_pMemoryAllocated = pMemeoryAllocated;
		for (int i = 0; i < m_numPages; i++) {
			PageIndex* pPageIndex = new PageIndex((Page*)((size_t)(pMemeoryAllocated)+(m_szPage*(m_numPages -1 - i))));
			pPageIndex->SetPNext(this->m_pHead);
			this->m_pHead = pPageIndex;
		}
	}

	PageIndex* Malloc(size_t szObject) {
		// find available pages
		size_t numPages = szObject / m_szPage + 1;

		// start page
		PageIndex* pStartPageIndex = this->m_pHead;
		PageIndex* pPageIndex = this->m_pHead;
		PageIndex* pLastPageIndex = this->m_pHead;

		size_t count = 0;
		while (pLastPageIndex != nullptr) {
			if (count == numPages) {
				PageIndex* pPageIndexAllocated = nullptr;
				if (pStartPageIndex == m_pHead) {
					pPageIndexAllocated = this->m_pHead;
					this->m_pHead = pLastPageIndex->GetPNext();
				} else {
					pPageIndexAllocated = pStartPageIndex->GetPNext();
					pStartPageIndex->SetPNext(pLastPageIndex->GetPNext());
				}
				pLastPageIndex->SetPNext(nullptr);
				// found
				return pPageIndexAllocated;
			}

			// at least two or more PageIndexs
			if (pPageIndex != m_pHead && pPageIndex != nullptr) {
				// if pages are not consecutive, reset count
				if ((size_t)(pLastPageIndex->GetPPage()) + m_szPage != (size_t)(pPageIndex->GetPPage())) {
					count = 0;
					PageIndex* pStartPageIndex = pLastPageIndex;
				}
			}

			// prceed to next PageIndex
			pLastPageIndex = pPageIndex;
			pPageIndex = pPageIndex->GetPNext();
			count++;
		}

		// allocated Pages
		throw Exception();
	}

	void Free(PageIndex* pPageIndexFree) {
		if (m_pHead == nullptr) {
			this->m_pHead = pPageIndexFree;
		}
		// find a position to free pages
		PageIndex* pPageIndex = m_pHead;
		PageIndex* pPositionToFree = pPageIndex;
		while (pPageIndex != nullptr) {
			if (pPageIndex->GetPPage() > pPageIndexFree->GetPPage()) {
				break;
			}
			pPositionToFree = pPageIndex;
			pPageIndex = pPageIndex->GetPNext();
		}

		// find a last PageIndex from "pPageIndexFree"
		pPageIndex = pPageIndexFree;
		PageIndex* pLastPageIndex = pPageIndex;
		while (pPageIndex != nullptr) {
			pLastPageIndex = pPageIndex;
			pPageIndex = pPageIndex->GetPNext();
		}

		// insert "pPageIndexFree" to "pPositionToFree"
		pPositionToFree->SetPNext(pPageIndexFree);
		pLastPageIndex->SetPNext(pPositionToFree->GetPNext());
	}

	size_t GetSzPage() { return this->m_szPage; }
	size_t GetNumPages() { return this->m_numPages; }

	void Show(const char* pTitle) {
		PageIndex* pCurrentPageIndex = m_pHead;
		PageIndex* pPreviousPageIndex = m_pHead;
		LOG_HEADER("PageManager::Show", 
			String(m_szMemoryAllocated), String(m_szPage), String((unsigned)(m_szMemoryAllocated)/m_szPage));
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
	PageManager* m_pPageManager;
	PageIndex* m_pPageIndex;

	Slot* m_pHead;
	SlotIndex* m_pNext;

	void GenerateSlots(Page* pPage) {
		size_t numSlots = (m_pPageManager->GetSzPage()* m_pPageManager->GetNumPages()) / this->m_szSlot;
		Slot* pSlot = (Slot*)pPage;
		Slot* pPreviousSlot = pSlot;
		for (int i = 0; i < numSlots; i++) {
			pSlot->pNext = (Slot*)((size_t)pSlot + m_szSlot);
			pPreviousSlot = pSlot;
			pSlot = pSlot->pNext;
		}
		pPreviousSlot->pNext = m_pHead;
		this->m_pHead = (Slot*)pPage;
	}

public:
	SlotIndex(size_t szSlot, PageManager* pPagemanager)
		: m_pNext(nullptr)
	{
		this->m_szSlot = szSlot;
		this->m_pPageManager = pPagemanager;
		PageIndex* pPageIndex = this->m_pPageManager->Malloc(this->m_szSlot);
		Page* pPage = pPageIndex->GetPPage();
		this->m_index = (size_t)pPage >> (size_t)(log2(this->m_pPageManager->GetSzPage()));

		this->GenerateSlots(pPage);
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

	bool Contains(void* pObject) {
	}

	// maintenance
	virtual void Show(const char* pTitle) {

		LOG_HEADER("SlotIndex::Show", String(m_szSlot), String((size_t)(m_pPageManager->GetSzPage() / m_szSlot)), String(m_pPageManager->GetSzPage()));
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

	SlotIndex* m_pHead;

public:
	SlotManager(size_t szSlot, PageManager* pPageManager) {
		// 
		this->m_pPageManager = pPageManager;

		// WORD size
		this->m_szWord = sizeof(size_t);
		this->m_szWordExponentOf2 = static_cast<size_t>(log2(static_cast<double>(this->m_szWord)));
		// PAGE size
		this->m_szPage = pPageManager->GetSzPage();
		this->m_szPageExponentOf2 = static_cast<size_t>(log2(static_cast<double>(this->m_szPage)));

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
			pSlotIndex = new SlotIndex(szSlot, m_pPageManager);
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
		pSlotIndex = new SlotIndex(szSlot, m_pPageManager);
		pSlotIndex->SetNext(pPreviousSlotIndex->GetNext());
		pPreviousSlotIndex->SetNext(pSlotIndex);
		return pSlotIndex->Malloc();
	}

	void Free(void* pObject) {
		SlotIndex* pSlotIndex = this->m_pHead;
		while (pSlotIndex != nullptr) {
			if (pSlotIndex->Contains(pObject)) {
				return pSlotIndex->Free(pObject);
			}
			pSlotIndex = pSlotIndex->GetNext();
		}
		throw Exception(static_cast<unsigned>(IMemory::EException::_eFree));
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

#define SIZE_PAGE 1024
#define SIZE_SLOT 8

class Memory :public IMemory
{
private:
	// components
	PageManager* m_pPageManager;
	SlotManager* m_pSlotManager;

protected:
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
	{
		this->m_pPageManager = new PageManager(SIZE_PAGE, szMemoryAllocated, pMemoryAllocated);
		this->m_pSlotManager = new SlotManager(SIZE_SLOT, this->m_pPageManager);
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
