#pragma once

#include "../typedef.h"
#include "../../01Base/Memory/IMemory.h"

class Slot {
public:
	Slot* pNext;
};

class SlotIndex : public IMemory {

private:
	size_t m_index;
	size_t m_szSlot;
	size_t m_numSlots;
	size_t m_numMaxSlots;
	PageManager* m_pPageManager;
	PageIndex* m_pPageIndex;

	Slot* m_pHead;
	SlotIndex* m_pNext;
	SlotIndex* m_pSibling;

	bool m_bGarbage;

	void GenerateSlots(PageManager* pPagemanager, size_t szSlot) {

		LOG_HEADER("SlotIndex::GenerateSlots(size)", szSlot);

		// check if szSlot is bigger than szPage
		size_t szPage = pPagemanager->GetSzPage();
		size_t numPagesAllocated = szSlot / szPage;
		if (szSlot > numPagesAllocated * szPage) {
			numPagesAllocated++;
		}
		this->m_pPageIndex = pPagemanager->Malloc(numPagesAllocated);
		Page* pPage = this->m_pPageIndex->GetPPage();
		this->m_index = this->m_pPageIndex->GetIndex();

		this->m_numMaxSlots = numPagesAllocated * szPage / szSlot;
		this->m_numSlots = this->m_numMaxSlots;

		this->m_pHead = (Slot*)pPage;
		Slot* pSlot = this->m_pHead;
		Slot* pPreviousSlot = pSlot;
		for (int i = 0; i < m_numMaxSlots; i++) {
			LOG("Slot-", (size_t)pSlot);
			pSlot->pNext = (Slot*)((size_t)pSlot + szSlot);
			pPreviousSlot = pSlot;
			pSlot = pSlot->pNext;
		}
		pPreviousSlot->pNext = nullptr;

		LOG_FOOTER("SlotIndex::GenerateSlots");
	}

public:
	SlotIndex(PageManager* pPagemanager, size_t szSlot)
		: m_pNext(nullptr)
		, m_pSibling(nullptr)
		, m_numMaxSlots(0)
		, m_numSlots(0)
		, m_bGarbage(false)
	{
		LOG_HEADER("SlotIndex::SlotIndex");
		this->m_pPageManager = pPagemanager;
		this->m_szSlot = szSlot;

		this->GenerateSlots(this->m_pPageManager, this->m_szSlot);
		LOG_FOOTER("SlotIndex::SlotIndex");
	}
	virtual ~SlotIndex() {
		this->m_pPageManager->Free(this->m_pPageIndex);
	}

//	size_t GetIndex() { return this->m_index; }
	size_t GetSzSlot() { return this->m_szSlot; }
	SlotIndex* GetPNext() { return this->m_pNext; }
	void SetPNext(SlotIndex* pNext) { this->m_pNext = pNext;	}
	SlotIndex* GetPSibling() { return this->m_pSibling; }
	void SetPSibling(SlotIndex* pSibling) { this->m_pSibling = pSibling; }
	bool IsGarbage() { return this->m_bGarbage; }

	Slot* Malloc(size_t szObject, SlotIndex *pPrevious) {
		// if the Slotindex of the same size is found
		if (m_szSlot == szObject) {
			LOG_HEADER("SlotIndex::Malloc");
			if (this->m_pHead == nullptr) {
				if (this->m_pSibling == nullptr) {
					// generate a new SlotIndex - Sibling
					this->m_pSibling = new SlotIndex(m_pPageManager, szObject);
				}
				Slot* pSlot = this->m_pSibling->Malloc(szObject, nullptr);
				return pSlot;

			} else {
				// allocate a slot
				Slot* pSlot = this->m_pHead;
				this->m_pHead = this->m_pHead->pNext;
				this->m_numSlots--;
				LOG_FOOTER("SlotIndex", (size_t)pSlot, m_numSlots);
				return pSlot;
			}
		}
		// if the slot size of current SlotIndex is less than needed, 
		// proceed to the next SlotIndex
		else if (m_szSlot < szObject) {
			if (m_pNext == nullptr) {
				// generate new SlotIndex of the same size - Sibling
				this->m_pNext = new SlotIndex(m_pPageManager, szObject);
			}
			Slot* pSlot = this->m_pNext->Malloc(szObject, this);
			return pSlot;
		}
		// if the slot size of current SlotIndex is grated than needed,
		// generate new SlotIndex
		else {
			SlotIndex* pNewSlotIndex = new SlotIndex(m_pPageManager, m_szSlot);
			pNewSlotIndex->SetPNext(this);
			pPrevious->SetPNext(pNewSlotIndex);
			Slot* pSlot = pNewSlotIndex->Malloc(szObject, this);
			return pSlot;
		}
	}
	bool Free(Slot* pSlotFree, size_t indexPage) {
		// found
		LOG_HEADER("SlotIndex::Free", (size_t)pSlotFree);
		if (indexPage == this->m_index) {
			// insert pSlotFree to Slot LIst
			pSlotFree->pNext = m_pHead;
			m_pHead = pSlotFree;
			this->m_numSlots++;
			if (m_numSlots == m_numMaxSlots) {
				// this is garbage
				this->m_bGarbage = true;
			}
			LOG_FOOTER("SlotIndex::Free1", (size_t)pSlotFree);
			return true;
		}
		else {
			// search in the sibling list
			if (this->m_pSibling != nullptr) {
				bool found = this->m_pSibling->Free(pSlotFree, indexPage);
				if (found) {
					if (this->m_pSibling->IsGarbage()) {
						SlotIndex* pGarbage = this->m_pSibling;
						this->m_pSibling = m_pSibling->GetPSibling();
						delete pGarbage;
					}
					LOG_FOOTER("SlotIndex::Free2", (size_t)pSlotFree);
					return false;
				}
			}
			// search in the next list
			if (this->m_pNext != nullptr) {
				bool found = this->m_pNext->Free(pSlotFree, indexPage);
				if (found) {
					if (this->m_pNext->IsGarbage()) {
						SlotIndex* pGarbage = this->m_pNext;
						this->m_pNext = pGarbage->GetPNext();
						delete pGarbage;
					}
					LOG_FOOTER("SlotIndex::Free3", (size_t)pSlotFree);
					return false;
				}
			}
		}
		throw Exception((unsigned)IMemory::EException::_eFree);
	}

	// maintenance
	virtual void Show(const char* pTitle) {
		LOG_HEADER("SlotIndex::Show", m_szSlot, (size_t)m_numSlots);
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
	size_t m_szPage;

	SlotIndex* m_pHead;

public:
	SlotManager(PageManager* pPageManager, size_t szWord) {
		// 
		this->m_pPageManager = pPageManager;

		// WORD size
		this->m_szWord = szWord;
		// PAGE size
		this->m_szPage = pPageManager->GetSzPage();
		this->m_pHead = nullptr;

		LOG_HEADER("SlotManager::SlotManager", (size_t)pPageManager);
		LOG("szWord, szPage", m_szWord, m_szPage);
		LOG_FOOTER("SlotManager");
	}
	virtual ~SlotManager() {
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
		// if any SlotIndex is not generated
		if (m_pHead == nullptr) {
			LOG("m_pHead == nullptr");
			m_pHead = new SlotIndex(m_pPageManager, szSlot);
			this->m_pHead = m_pHead;
			Slot* pSlot = m_pHead->Malloc(szObject, nullptr);
			LOG_FOOTER("SlotManager1", (size_t)pSlot);
			return pSlot;
		}
		else {
			LOG("m_pHead != nullptr");
			Slot* pSlot = this->m_pHead->Malloc(szObject, nullptr);
			LOG_FOOTER("SlotManager2", (size_t)pSlot);
			return pSlot;
		}
	}

	void Free(void* pObject) {
		size_t indexPage = (size_t)pObject >> (size_t)(log2((double)this->m_pPageManager->GetSzPage()));
		bool found = this->m_pHead->Free((Slot*)pObject, indexPage);
		// if m_pHead is a target SlotIndex
		LOG_HEADER("SlotManager::Free", (size_t)pObject);
		if (found) {
			// if m_pHead is a Garbage
			if (this->m_pHead->IsGarbage()) {
				// promote pSibling and delete m_pHead
				if (this->m_pHead->GetPSibling() != nullptr) {
					SlotIndex* pGarbage = this->m_pHead;
					this->m_pHead->GetPSibling()->SetPNext(this->m_pHead);
					this->m_pHead = this->m_pHead->GetPSibling();
					delete pGarbage;
				}
				else {
					// delete m_pHead
					SlotIndex* pGarbage = this->m_pHead;
					this->m_pHead = pGarbage->GetPNext();
					delete pGarbage;
				}
			}
		}
		LOG_FOOTER("SlotManager::Free", (size_t)pObject);
	}

	// maintenance
	virtual void Show(const char* pTitle) {
		this->m_pPageManager->Show("");

		LOG_HEADER("SlotManager::Show");
		SlotIndex* pSlotIndex = this->m_pHead;
		while (pSlotIndex != nullptr) {
			pSlotIndex->Show("");
			pSlotIndex = pSlotIndex->GetPNext();
		}
		LOG_FOOTER("SlotManager");
	}
};

