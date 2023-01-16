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


		LOG_HEADER("PageManager::PageManager", m_numPages, m_szPage, szMemoryAllocated);
		PageIndex* pPageIndex = this->m_pHead;
		while (pPageIndex != nullptr) {
			LOG("Page", (size_t)pPageIndex->GetPPage());
			pPageIndex = pPageIndex->GetPNext();
		}
		LOG_FOOTER("PageManager");
	}

	PageIndex* Malloc(size_t numPagesAllocated) {
			// find available pages
		m_numPages -= numPagesAllocated;

		// start page
		PageIndex* pStartPageIndex = this->m_pHead;
		PageIndex* pPageIndex = this->m_pHead;
		PageIndex* pLastPageIndex = this->m_pHead;

		LOG_HEADER("PageManager::Malloc", numPagesAllocated, m_numPages);

		size_t count = 0;
		while (pLastPageIndex != nullptr) {
			if (count == numPagesAllocated) {
				PageIndex* pPageIndexAllocated = nullptr;
				if (pStartPageIndex == m_pHead) {
					pPageIndexAllocated = this->m_pHead;
					this->m_pHead = pLastPageIndex->GetPNext();
				} else {
					pPageIndexAllocated = pStartPageIndex->GetPNext();
					pStartPageIndex->SetPNext(pLastPageIndex->GetPNext());
				}
				pLastPageIndex->SetPNext(nullptr);
				LOG_FOOTER("PageManager::Malloc", (size_t)pPageIndexAllocated);
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
		LOG_HEADER("PageManager::Show", m_szMemoryAllocated, m_szPage, m_numPages);
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

	void GenerateSlots(size_t szSlot, PageManager* pPagemanager) {
		// check if szSlot is bigger than szPage
		size_t szPage = pPagemanager->GetSzPage();
		size_t numPagesAllocated = szSlot / szPage;
		if (szSlot > numPagesAllocated * szPage) {
			numPagesAllocated++;
		}
		PageIndex* pPageIndex = pPagemanager->Malloc(numPagesAllocated);
		Page* pPage = pPageIndex->GetPPage();
		this->m_index = (size_t)pPage >> (size_t)log2(szPage);

		size_t numSlots = numPagesAllocated * szPage / szSlot;


		this->m_pHead = (Slot*)pPage;
		Slot* pSlot = this->m_pHead;
		Slot* pPreviousSlot = pSlot;
		LOG_HEADER("SlotIndex::GenerateSlots", numPagesAllocated, szPage);
		LOG(szSlot, numSlots);
		for (int i = 0; i < numSlots; i++) {
			LOG("Slot-", String(size_t(pSlot)));
			pSlot->pNext = (Slot*)((size_t)pSlot + szSlot);
			pPreviousSlot = pSlot;
			pSlot = pSlot->pNext;
		}
		pPreviousSlot->pNext = nullptr;
		LOG_FOOTER("SlotIndex::GenerateSlots");
	}

public:
	SlotIndex(size_t szSlot, PageManager* pPagemanager)
		: m_pNext(nullptr)
	{
		LOG_HEADER("SlotIndex::SlotIndex");
		this->m_szSlot = szSlot;
		this->m_pPageManager = pPagemanager;
		this->GenerateSlots(szSlot, pPagemanager);
		LOG_FOOTER("SlotIndex::SlotIndex");
	}
	virtual ~SlotIndex() {
	}

	size_t GetIndex() { return this->m_index; }
	size_t GetSzSlot() { return this->m_szSlot; }
	SlotIndex* GetNext() { return this->m_pNext; }
	void SetNext(SlotIndex* pNext) { this->m_pNext = pNext;	}

	Slot* Malloc() { 
		LOG_HEADER("SlotIndex::Malloc");
		if (this->m_pHead == nullptr) {
			return nullptr;
		}
		Slot* pSlot = this->m_pHead;
		this->m_pHead = this->m_pHead->pNext;
		LOG_FOOTER("SlotIndex", (size_t)pSlot);
		return pSlot;
	}
	void Free(void* pObject) {
		Slot* pSlotFree = (Slot*)pObject;
		pSlotFree->pNext = m_pHead;
		m_pHead = pSlotFree;
	}

	bool Contains(void* pObject) {
		size_t indexObject = (size_t)pObject >> (size_t)(log2(this->m_pPageManager->GetSzPage()));
		if (indexObject == this->GetIndex()) {
			return true;
		}
		return false;

	}

	// maintenance
	virtual void Show(const char* pTitle) {

		LOG_HEADER("SlotIndex::Show", String(m_szSlot), String((size_t)(m_pPageManager->GetSzPage() / m_szSlot)), String(m_pPageManager->GetSzPage()));
		Slot* pSlot = this->m_pHead;
		while (pSlot != nullptr) {
			LOG("Slot-", String(size_t(pSlot)));
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
		LOG_HEADER("SlotManager::SlotManager");
		LOG(m_szWord, m_szWordExponentOf2, m_szPage, m_szPageExponentOf2);
		LOG_FOOTER("SlotManager");
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

		LOG_HEADER("SlotManager::Malloc", szObject, szSlot);
		SlotIndex* pSlotIndex = this->m_pHead;
		// if any SlotIndex is not generated
		if (pSlotIndex == nullptr) {
			pSlotIndex = new SlotIndex(szSlot, m_pPageManager);
			this->m_pHead = pSlotIndex;
			Slot* pSlot = pSlotIndex->Malloc();
			LOG_FOOTER("SlotManager1", (size_t)pSlot);
			return pSlot;
		}
		// find SlotIndex
		SlotIndex* pPreviousSlotIndex = this->m_pHead;
		while (pSlotIndex != nullptr) {
			// if slotList is arleady generated
			if (pSlotIndex->GetSzSlot() == szSlot) {
				Slot* pSlot = pSlotIndex->Malloc();
				LOG_FOOTER("SlotManager2", (size_t)pSlot);
				return pSlot;
			}
			// insert new SlotIndex
			else if (pSlotIndex->GetSzSlot() > szSlot) {
				break;
			}
			pPreviousSlotIndex = pSlotIndex;
			pSlotIndex = pSlotIndex->GetNext();
		}
		// if slotlist of the size is not generated

		// insert new SlotIndex
		pSlotIndex = new SlotIndex(szSlot, m_pPageManager);
		pSlotIndex->SetNext(pPreviousSlotIndex->GetNext());
		pPreviousSlotIndex->SetNext(pSlotIndex);
		Slot* pSlot = pSlotIndex->Malloc();
		LOG_FOOTER("SlotManager3", (size_t)pSlot);

		return pSlot;
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

#define SIZE_PAGE 256
#define SIZE_WORD 8

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
		LOG_HEADER("Memory::Memory");
		LOG((size_t)pMemoryAllocated, szMemoryAllocated, SIZE_PAGE, SIZE_WORD);
		this->m_pPageManager = new PageManager(SIZE_PAGE, szMemoryAllocated, pMemoryAllocated);
		this->m_pSlotManager = new SlotManager(SIZE_WORD, this->m_pPageManager);
		LOG_FOOTER("Memory");
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
