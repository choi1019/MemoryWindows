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
		LOG_HEADER("PageIndex::show", String((size_t)m_pPage), String((size_t)m_pNext));
		LOG_FOOTER("PageIndex");
	}

};

class PageManager : public IMemory {
private:
	size_t m_szPage;
	size_t m_szMemoryAllocated;
	void* m_pMemoryAllocated;

	// pageIndex
	PageIndex* m_pHead;

public:
	PageManager(size_t szPage, size_t szMemoryAllocated, void *pMemeoryAllocated) {
		this->m_szPage = szPage;
		this->m_szMemoryAllocated = szMemoryAllocated;
		if (this->m_szMemoryAllocated < m_szPage) {
			throw Exception(static_cast<unsigned>(IMemory::EException::_eMemoryAllocatedIsSmallerThanAPage));
		}
		this->m_pMemoryAllocated = pMemeoryAllocated;

		size_t numPages = szMemoryAllocated / szPage;
		for (int i = 0; i < numPages; i++) {
			PageIndex pPageIndex = new("") PageIndex((Page*)pMemeoryAllocated);
			this->m_pHead 
			pPage->pNext = (Slot*)((size_t)pSlot + m_szSlot);
			pPreviousSlot = pPage;
			pPage = pPage->pNext;
		}
		pPreviousSlot->pNext = m_pHead;
		this->m_pHead = (Slot*)pPage;
	}

	Page* Malloc(size_t szObject) {
		if (this->m_pHead == nullptr) {
			throw Exception((unsigned)IMemory::EException::_eNoMoreSlot);
		}
		size_t numPages = szObject / m_szPage + 1;
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

	size_t GetSzPage() { return this->m_szPage; }

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
	PageManager* m_pPageManager;
	PageIndex* m_pPageIndex;

	Slot* m_pHead;
	SlotIndex* m_pNext;

	void AddPage(Page* pPage) {
		size_t numSlots = m_pPageManager->GetSzPage() / this->m_szSlot;
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
		Page* pPage = this->m_pPageManager->Malloc(this->m_szSlot);

		this->m_index = (size_t)pPage >> (size_t)(log2(this->m_pPageManager->GetSzPage()));
		this->m_pHead = nullptr;
		this->AddPage(pPage);
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
