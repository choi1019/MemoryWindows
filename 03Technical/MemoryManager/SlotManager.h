#pragma once

#include "../typedef.h"
#include "../../01Base/Memory/IMemory.h"
#include <math.h>

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

	void GenerateSlots(PageManager* pPagemanager, size_t szSlot) {
		// check if szSlot is bigger than szPage
		size_t szPage = pPagemanager->GetSzPage();
		size_t numPagesAllocated = szSlot / szPage;
		if (szSlot > numPagesAllocated * szPage) {
			numPagesAllocated++;
		}

		PageIndex* pPageIndex = pPagemanager->Malloc(numPagesAllocated);
		this->m_pPageIndex = pPageIndex;
		Page* pPage = this->m_pPageIndex->GetPPage();
		this->m_index = ((size_t)pPage >> (size_t)log2((double)szPage));

		this->m_numMaxSlots = numPagesAllocated * szPage / szSlot;
		this->m_numSlots = this->m_numMaxSlots;


		this->m_pHead = (Slot*)pPage;
		Slot* pSlot = this->m_pHead;
		Slot* pPreviousSlot = pSlot;

		LOG_HEADER("SlotIndex::GenerateSlots", numPagesAllocated, szPage);
		LOG(szSlot, m_numMaxSlots);
		for (int i = 0; i < m_numMaxSlots; i++) {
			LOG("Slot-", String(size_t(pSlot)));
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

	Slot* Malloc(size_t szObject, SlotIndex *pPrevious) { 
		if (m_szSlot == szObject) {
			LOG_HEADER("SlotIndex::Malloc");
			if (this->m_pHead == nullptr) {
				if (this->m_pSibling == nullptr) {
					// generate new SlotIndex of the same size - Sibling
					SlotIndex* pNewSlotIndex = new SlotIndex(m_pPageManager, szObject);
					this->m_pSibling = pNewSlotIndex;
				}
				Slot* pSlot = this->m_pSibling->Malloc(szObject, nullptr);
				return pSlot;

			} else {
				// get a slot
				Slot* pSlot = this->m_pHead;
				this->m_pHead = this->m_pHead->pNext;
				this->m_numSlots--;
				LOG_FOOTER("SlotIndex", (size_t)pSlot, m_numSlots);
				return pSlot;
			}
		}
		else if (m_szSlot < szObject) {
			if (m_pNext == nullptr) {
				// generate new SlotIndex of the same size - Sibling
				SlotIndex* pNewSlotIndex = new SlotIndex(m_pPageManager, szObject);
				this->m_pNext = pNewSlotIndex;
				Slot* pSlot = this->m_pNext->Malloc(szObject, this);
				return pSlot;
			} else {
				Slot* pSlot = this->m_pNext->Malloc(szObject, this);
				return pSlot;
			}
		}
		else {
			SlotIndex* pNewSlotIndex = new SlotIndex(m_pPageManager, m_szSlot);
			pNewSlotIndex->SetPNext(this);
			pPrevious->SetPNext(pNewSlotIndex);
			Slot* pSlot = pNewSlotIndex->Malloc(szObject, this);
			return pSlot;
		}
	}
	SlotIndex* Free(Slot* pSlotFree) {
		size_t indexSlot = (size_t)pSlotFree >> (size_t)(log2((double)this->m_pPageManager->GetSzPage()));
		// found
		if (indexSlot == this->m_index) {
			// insert pSlotFree to Slot LIst
			pSlotFree->pNext = m_pHead;
			m_pHead = pSlotFree;
			this->m_numSlots++;
			if (m_numSlots == m_numMaxSlots) {
				// this is garbage
				return this;
			}
		}
		// search in the sibling list
		if (this->m_pSibling != nullptr) {
			SlotIndex* pGarbage = this->m_pSibling->Free(pSlotFree);
			if (pGarbage != nullptr) {
				this->m_pSibling = pGarbage->GetPSibling();
				delete pGarbage;
			}
		}
		// search in the next list
		if (this->m_pNext != nullptr) {
			SlotIndex* pGarbage = this->m_pNext->Free(pSlotFree);
			if (pGarbage != nullptr) {
				this->m_pNext = pGarbage->GetPNext();
				delete pGarbage;
			}
		}
		return nullptr;
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
	SlotManager(PageManager* pPageManager, size_t szWord) {
		// 
		this->m_pPageManager = pPageManager;

		// WORD size
		this->m_szWord = szWord;
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
			pSlotIndex = new SlotIndex(m_pPageManager, szSlot);
			this->m_pHead = pSlotIndex;
			Slot* pSlot = pSlotIndex->Malloc(szObject, nullptr);
			LOG_FOOTER("SlotManager1", (size_t)pSlot);
			return pSlot;
		}

		Slot* pSlot = this->m_pHead->Malloc(szObject, nullptr);
		LOG_FOOTER("SlotManager3", (size_t)pSlot);
		return pSlot;

		/*
		// find SlotIndex to insert
		SlotIndex* pSlotIndexToInsert = this->m_pHead;
		while (pSlotIndex != nullptr) {
			// if the slotIndex is arleady in the list
			if (pSlotIndex->GetSzSlot() == szSlot) {
				Slot* pSlot = pSlotIndex->Malloc();
				LOG_FOOTER("SlotManager2", (size_t)pSlot);
				return pSlot;
			}
			// insert new SlotIndex
			else if (pSlotIndex->GetSzSlot() > szSlot) {
				break;
			}
			pSlotIndexToInsert = pSlotIndex;
			pSlotIndex = pSlotIndex->GetPNext();
		}
		// if slotlist of the size is not generated

		// insert new SlotIndex
		pSlotIndex = new SlotIndex(m_pPageManager, szSlot);
		pSlotIndex->SetPNext(pSlotIndexToInsert->GetPNext());
		pSlotIndexToInsert->SetPNext(pSlotIndex);

		Slot* pSlot = pSlotIndex->Malloc();
		*/
	}

	void Free(void *pObject) {
		SlotIndex *pGarbage = this->m_pHead->Free((Slot*)pObject);
		if (pGarbage != nullptr) {
			this->m_pHead = pGarbage->GetPNext();
			delete pGarbage;
		}
		/*
		Slot* pSlotToFree = (Slot *)pObject;
		SlotIndex* pSlotIndex = this->m_pHead;
		SlotIndex* pPreviousSlotIndex = this->m_pHead;
		while (pSlotIndex != nullptr) {
			SlotIndex* pTargetSlotIndex = pSlotIndex->Contains(pSlotToFree);
			// if pObject is contained in pTargetSlotIndex
			if (pTargetSlotIndex != nullptr) {
				// if pTargetSlotIndex is not garbage
				if (pTargetSlotIndex->Free(pSlotToFree)) {
					return;
				}
				// if pTargetSlotIndex is garbage
				else {
					SlotIndex* pNewSlotIndex = pSlotIndex->RemoveFromSiblingList(pTargetSlotIndex);
					if (pNewSlotIndex == nullptr) {

					} else {

					}
					delete pTargetSlotIndex;
				}
			}
			pPreviousSlotIndex = pSlotIndex;
			pSlotIndex = pSlotIndex->GetNext();
		}
		throw Exception(static_cast<unsigned>(IMemory::EException::_eFree));
		*/
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

