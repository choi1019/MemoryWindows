#pragma once

#include "../typedef.h"
//#include "../../01Base/Object/BaseObject.h"
#include "SystemMemoryObject.h"

#include "PageManager.h"

class Slot {
public:
	Slot* pNext;
};

class SLotList : public SystemMemoryObject {

private:
	size_t m_szSlot;

	size_t m_index;
	size_t m_numSlots;
	size_t m_numMaxSlots;

	PageIndex* m_pPageIndex;
	Slot* m_pHead;
	SLotList* m_pNext;
	SLotList* m_pSibling;

	PageManager* m_pPageManager;
	bool m_bGarbage;

public:
	SLotList(size_t szSlot)
		: m_szSlot(szSlot)

		, m_index(0)
		, m_numSlots(0)
		, m_numMaxSlots(0)

		, m_pPageIndex(nullptr)
		, m_pHead(nullptr)
		, m_pNext(nullptr)
		, m_pSibling(nullptr)

		, m_pPageManager(nullptr)
		, m_bGarbage(false)
	{
		LOG_HEADER("SLotList::SLotList", m_szSlot);
		LOG_FOOTER("SLotList::SLotList");
	}
	virtual ~SLotList() {
		this->m_pPageManager->Free(this->m_pPageIndex);
	}
	virtual void Initialize(PageManager* pPagemanager) {
		LOG_HEADER("SLotList::Initialize(size)", m_szSlot);

		this->m_pPageManager = pPagemanager;
		// check if szSlot is bigger than szPage
		size_t szPage = m_pPageManager->GetSzPage();
		size_t numPagesAllocated = m_szSlot / szPage;
		if (m_szSlot > numPagesAllocated * szPage) {
			numPagesAllocated++;
		}
		this->m_pPageIndex = m_pPageManager->Malloc(numPagesAllocated);
		Page* pPage = this->m_pPageIndex->GetPPage();
		this->m_index = this->m_pPageIndex->GetIndex();

		this->m_numMaxSlots = numPagesAllocated * szPage / m_szSlot;
		this->m_numSlots = this->m_numMaxSlots;

		this->m_pHead = (Slot*)pPage;
		Slot* pSlot = this->m_pHead;
		Slot* pPreviousSlot = pSlot;
		for (int i = 0; i < m_numSlots; i++) {
			LOG_NEWLINE("Slot-", (size_t)pSlot);
			pSlot->pNext = (Slot*)((size_t)pSlot + m_szSlot);
			pPreviousSlot = pSlot;
			pSlot = pSlot->pNext;
		}
		pPreviousSlot->pNext = nullptr;

		LOG_FOOTER("SLotList::Initialize", m_numSlots);
	}
	virtual void Finalize() {
	}

	PageIndex* GetPPageIndex() { return this->m_pPageIndex; }
	size_t GetSzSlot() { return this->m_szSlot; }
	SLotList* GetPNext() { return this->m_pNext; }
	void SetPNext(SLotList* pNext) { this->m_pNext = pNext;	}
	SLotList* GetPSibling() { return this->m_pSibling; }
	void SetPSibling(SLotList* pSibling) { this->m_pSibling = pSibling; }
	bool IsGarbage() { return this->m_bGarbage; }

	Slot* Malloc(size_t szObject, SLotList *pPrevious) {
		// if the SLotList of the same size is found
		LOG_HEADER("SLotList::Malloc");
		if (m_szSlot == szObject) {
			LOG_NEWLINE("m_szSlot == szObject", "found");
			if (this->m_pHead == nullptr) {
				if (this->m_pSibling == nullptr) {
					// generate a new SLotList - Sibling
					this->m_pSibling = new("") SLotList(szObject);
					this->m_pSibling->Initialize(m_pPageManager);
				}
				Slot* pSlot = this->m_pSibling->Malloc(szObject, nullptr);
				LOG_FOOTER("SLotList::Malloc1", (size_t)pSlot, m_numSlots);
				return pSlot;

			} else {
				// allocate a slot
				Slot* pSlot = this->m_pHead;
				this->m_pHead = this->m_pHead->pNext;
				this->m_numSlots--;
				LOG_FOOTER("SLotList::Malloc2", (size_t)pSlot, m_numSlots);
				return pSlot;
			}
		}
		// if the slot size of current SLotList is less than needed, 
		// proceed to the next SLotList
		else if (m_szSlot < szObject) {
			LOG_NEWLINE("m_szSlot < szObject", "found");
			if (m_pNext == nullptr) {
				// generate new SLotList of the same size - Sibling
				this->m_pNext = new("") SLotList(szObject);
				this->m_pNext->Initialize(m_pPageManager);
			}
			Slot* pSlot = this->m_pNext->Malloc(szObject, this);
			LOG_FOOTER("SLotList::Malloc3", (size_t)pSlot, m_numSlots);
			return pSlot;
		}
		// if the slot size of current SLotList is grated than the required size,
		// generate new SLotList
		else {
			LOG_NEWLINE("m_szSlot > szObject", "found");
			SLotList* pNewSLotList = new("") SLotList(m_szSlot);
			pNewSLotList->Initialize(m_pPageManager);

			pNewSLotList->SetPNext(this);
			pPrevious->SetPNext(pNewSLotList);
			Slot* pSlot = pNewSLotList->Malloc(szObject, this);
			LOG_FOOTER("SLotList::Malloc4", (size_t)pSlot, m_numSlots);
			return pSlot;
		}
	}
	bool Free(Slot* pSlotFree, size_t indexPage) {
		// found
		LOG_HEADER("SLotList::Free", (size_t)pSlotFree, (size_t)m_pPageIndex->GetPPage());
		if (indexPage == this->m_index) {
			LOG_NEWLINE("indexPage == this->m_index");
			// insert pSlotFree to Slot LIst
			pSlotFree->pNext = m_pHead;
			m_pHead = pSlotFree;
			this->m_numSlots++;
			if (m_numSlots == m_numMaxSlots) {
				// this is garbage
				this->m_bGarbage = true;
			}
			LOG_FOOTER("SLotList::Free1", (size_t)pSlotFree);
			return true;
		}
		else {
			LOG_NEWLINE("indexPage != this->m_index");
			// search in the sibling list
			if (this->m_pSibling != nullptr) {
				LOG_NEWLINE("this->m_pSibling != nullptr");
				bool found = this->m_pSibling->Free(pSlotFree, indexPage);
				if (found) {
					if (this->m_pSibling->IsGarbage()) {
						SLotList* pGarbage = this->m_pSibling;
						this->m_pSibling = m_pSibling->GetPSibling();
						delete pGarbage;
					}
					LOG_FOOTER("SLotList::Free2", (size_t)pSlotFree);
					return false;
				}
			}
			// search in the next list
			if (this->m_pNext != nullptr) {
				LOG_NEWLINE("this->m_pNext != nullptr");
				bool found = this->m_pNext->Free(pSlotFree, indexPage);
				if (found) {
					LOG_NEWLINE("Found");
					if (this->m_pNext->IsGarbage()) {
						SLotList* pGarbage = this->m_pNext;
						if (pGarbage->GetPSibling() != nullptr) {
							pGarbage->GetPSibling()->SetPNext(pGarbage->GetPNext());
							this->SetPNext(pGarbage->GetPSibling());
						} else {
							this->m_pNext = pGarbage->GetPNext();
						}
						LOG_NEWLINE("this->m_pNext->IsGarbage()", (size_t)pGarbage, (size_t)pGarbage->GetPPageIndex()->GetPPage());
						delete pGarbage;
					}
					LOG_FOOTER("SLotList::Free3", (size_t)pSlotFree);
					return false;
				}
			}
		}
		throw Exception((unsigned)IMemory::EException::_eFree);
	}

	// maintenance
	virtual void Show(const char* pTitle) {
		LOG_HEADER("SLotList::Show", pTitle, m_szSlot, (size_t)m_numSlots);
		Slot* pSlot = this->m_pHead;
		while (pSlot != nullptr) {
			LOG_NEWLINE("Slot-", (size_t)pSlot);
			pSlot = pSlot->pNext;
		}
		LOG_FOOTER("SLotList::Show", pTitle);
	};
};

class SlotManager : public SystemMemoryObject {

private:
	PageManager* m_pPageManager;

	size_t m_szWord;
	size_t m_szPage;

	SLotList* m_pHead;

public:
	SlotManager(size_t szWord) {
		LOG_HEADER("SlotManager::SlotManager", szWord);

		// WORD size
		this->m_szWord = szWord;
		// PAGE size
		this->m_szPage = 0;
		this->m_pHead = nullptr;
		this->m_pPageManager = m_pPageManager;

		LOG_NEWLINE("szWord", m_szWord);
		LOG_FOOTER("SlotManager");
	}
	virtual ~SlotManager() {
	}

	virtual void Initialize(PageManager* pPageManager) {
		this->m_pPageManager = pPageManager;
		this->m_szPage = pPageManager->GetSzPage();
		LOG_HEADER("SlotManager::Initialize", m_szPage);
		LOG_FOOTER("SlotManager::Initialize");
	}
	virtual void Finalize() {

	}

	void* Malloc(size_t szObject) {
		size_t szSlot = szObject;

		// multiple of WORD
		size_t szWordExponentOf2 = (size_t)(log2(static_cast<double>(this->m_szWord)));
		szSlot >>= szWordExponentOf2;
		szSlot <<= szWordExponentOf2;
		if (szSlot < szObject) {
			szSlot += m_szWord;
		}

		LOG_HEADER("SlotManager::Malloc", szObject, szSlot);
		// if any SLotList is not generated
		if (m_pHead == nullptr) {
			LOG_NEWLINE("m_pHead == nullptr");
			m_pHead = new("") SLotList(szSlot);
			m_pHead->Initialize(m_pPageManager);

			this->m_pHead = m_pHead;
			Slot* pSlot = m_pHead->Malloc(szObject, nullptr);
			LOG_FOOTER("SlotManager::Malloc1", (size_t)pSlot);
			return pSlot;
		}
		else {
			LOG_NEWLINE("m_pHead != nullptr");
			Slot* pSlot = this->m_pHead->Malloc(szObject, nullptr);
			LOG_FOOTER("SlotManager::Malloc2", (size_t)pSlot);
			return pSlot;
		}
	}

	void Free(void* pObject) {
		LOG_HEADER("SlotManager::Free", (size_t)pObject);
		size_t indexPage = (size_t)pObject >> (size_t)(log2((double)this->m_pPageManager->GetSzPage()));
		bool found = this->m_pHead->Free((Slot*)pObject, indexPage);
		// if m_pHead is a target SLotList
		if (found) {
			LOG_NEWLINE("found");
			// if m_pHead is a Garbage
			if (this->m_pHead->IsGarbage()) {
				LOG_NEWLINE("if (this->m_pHead->IsGarbage())");
				SLotList* pGarbage = this->m_pHead;
				if (this->m_pHead->GetPSibling() != nullptr) {
					// promote pSibling and delete pGarbage
					this->m_pHead = pGarbage->GetPSibling();
					this->m_pHead->SetPNext(pGarbage->GetPNext());
				}
				else {
					this->m_pHead = pGarbage->GetPNext();
				}
				delete pGarbage;
			}
		}
		LOG_FOOTER("SlotManager::Free", (size_t)pObject);
	}

	// maintenance
	virtual void Show(const char* pTitle) {
		this->m_pPageManager->Show("");

		LOG_HEADER("SlotManager::Show");
		SLotList* pSLotList = this->m_pHead;
		while (pSLotList != nullptr) {
			pSLotList->Show("Next");
			SLotList *pSiblingSLotList = pSLotList->GetPSibling();
			while (pSiblingSLotList != nullptr) {
				pSiblingSLotList->Show("Sibling");
				pSiblingSLotList = pSiblingSLotList->GetPSibling();
			}
			pSLotList = pSLotList->GetPNext();
		}
		LOG_FOOTER("SlotManager");
	}
};

