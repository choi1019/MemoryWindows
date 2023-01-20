#pragma once

#include "../typedef.h"
#include "SystemMemoryObject.h"

#include "PageList.h"

class Slot {
public:
	Slot* pNext;
};

class SlotList : public SystemMemoryObject {
private:
	static SlotList* s_pFreeSlotList;

public:
	void* operator new(size_t szThis, const char* sMessage) {
		void* pNewSlotList = nullptr;
		if (s_pFreeSlotList == nullptr) {
			pNewSlotList = s_pCurrentSystemMemoryAllocated;
			s_pCurrentSystemMemoryAllocated = (void*)((size_t)s_pCurrentSystemMemoryAllocated + sizeof(SlotList));
		}
		else {
			pNewSlotList = s_pFreeSlotList;
			s_pFreeSlotList = s_pFreeSlotList->GetPNext();
		}
		return pNewSlotList;
	}
	void operator delete(void* pObject) {
		SlotList* pSlotList = (SlotList*) pObject;
		pSlotList->SetPNext(s_pFreeSlotList);
		s_pFreeSlotList = pSlotList;
	}
	void operator delete(void* pObject, const char* sMessage) {
		SlotList* pSlotList = (SlotList*)pObject;
		pSlotList->SetPNext(s_pFreeSlotList);
		s_pFreeSlotList = pSlotList;
	}

private:
	size_t m_szSlot;

	size_t m_index;
	size_t m_numSlots;
	size_t m_numMaxSlots;

	PageIndex* m_pPageIndex;
	Slot* m_pHead;
	SlotList* m_pNext;
	SlotList* m_pSibling;

	PageList* m_pPageList;
	bool m_bGarbage;

public:
	SlotList(size_t szSlot, PageList* pPageList)
		: m_szSlot(szSlot)
		, m_pPageList(pPageList)

		, m_index(0)
		, m_numSlots(0)
		, m_numMaxSlots(0)

		, m_pPageIndex(nullptr)
		, m_pHead(nullptr)
		, m_pNext(nullptr)
		, m_pSibling(nullptr)

		, m_bGarbage(false)
	{
		LOG_HEADER("SlotList::SlotList(m_szSlot, m_pPageList)", m_szSlot, (size_t)m_pPageList);
		// check if szSlot is bigger than szPage
		size_t szPage = m_pPageList->GetSzPage();
		size_t numPagesAllocated = m_szSlot / szPage;
		if (m_szSlot > numPagesAllocated * szPage) {
			numPagesAllocated++;
		}
		this->m_pPageIndex = m_pPageList->Malloc(numPagesAllocated);
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

		LOG_FOOTER("SlotList::SlotList");
	}
	virtual ~SlotList() {
		this->m_pPageList->Free(this->m_pPageIndex);
	}
	virtual void Initialize() {
		LOG_HEADER("SlotList::Initialize(size)");
		LOG_FOOTER("SlotList::Initialize");
	}
	virtual void Finalize() {
	}

	PageIndex* GetPPageIndex() { return this->m_pPageIndex; }
	size_t GetSzSlot() { return this->m_szSlot; }
	SlotList* GetPNext() { return this->m_pNext; }
	void SetPNext(SlotList* pNext) { this->m_pNext = pNext;	}
	SlotList* GetPSibling() { return this->m_pSibling; }
	void SetPSibling(SlotList* pSibling) { this->m_pSibling = pSibling; }
	bool IsGarbage() { return this->m_bGarbage; }

	Slot* Malloc(size_t szObject, SlotList *pPrevious) {
		// if the SlotList of the same size is found
		LOG_HEADER("SlotList::Malloc(szObject, pPrevious)", szObject, (size_t)pPrevious);
		if (m_szSlot == szObject) {
			LOG_NEWLINE("m_szSlot == szObject", "found");
			if (this->m_pHead == nullptr) {
				if (this->m_pSibling == nullptr) {
					// generate a new SlotList - Sibling
					this->m_pSibling = new("") SlotList(szObject, m_pPageList);
				}
				Slot* pSlot = this->m_pSibling->Malloc(szObject, nullptr);
				LOG_FOOTER("SlotList::Malloc1(pSlot, m_numSlots)", (size_t)pSlot, m_numSlots);
				return pSlot;

			} else {
				// allocate a slot
				Slot* pSlot = this->m_pHead;
				this->m_pHead = this->m_pHead->pNext;
				this->m_numSlots--;
				LOG_FOOTER("SlotList::Malloc2(pSlot, m_numSlots)", (size_t)pSlot, m_numSlots);
				return pSlot;
			}
		}
		// if the slot size of current SlotList is less than needed, 
		// proceed to the next SlotList
		else if (m_szSlot < szObject) {
			LOG_NEWLINE("m_szSlot < szObject", "found");
			if (m_pNext == nullptr) {
				// generate new SlotList of the same size - Sibling
				this->m_pNext = new("") SlotList(szObject, m_pPageList);
			}
			Slot* pSlot = this->m_pNext->Malloc(szObject, this);
			LOG_FOOTER("SlotList::Malloc3(pSlot, m_numSlots)", (size_t)pSlot, m_numSlots);
			return pSlot;
		}
		// if the slot size of current SlotList is grated than the required size,
		// generate new SlotList
		else {
			LOG_NEWLINE("m_szSlot > szObject", "found");
			SlotList* pNewSlotList = new("") SlotList(m_szSlot, m_pPageList);

			pNewSlotList->SetPNext(this);
			pPrevious->SetPNext(pNewSlotList);
			Slot* pSlot = pNewSlotList->Malloc(szObject, this);
			LOG_FOOTER("SlotList::Malloc4(pSlot, m_numSlots)", (size_t)pSlot, m_numSlots);
			return pSlot;
		}
	}
	bool Free(Slot* pSlotFree, size_t indexPage) {
		// found
		LOG_HEADER("SlotList::Free(pSlotFree, indexPage)", (size_t)pSlotFree, indexPage);
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
			LOG_FOOTER("SlotList::Free1(pSlotFree)", (size_t)pSlotFree);
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
						SlotList* pGarbage = this->m_pSibling;
						this->m_pSibling = m_pSibling->GetPSibling();
						delete pGarbage;
					}
					LOG_FOOTER("SlotList::Free2(pSlotFree)", (size_t)pSlotFree);
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
						SlotList* pGarbage = this->m_pNext;
						if (pGarbage->GetPSibling() != nullptr) {
							pGarbage->GetPSibling()->SetPNext(pGarbage->GetPNext());
							this->SetPNext(pGarbage->GetPSibling());
						} else {
							this->m_pNext = pGarbage->GetPNext();
						}
						LOG_NEWLINE("this->m_pNext->IsGarbage()", (size_t)pGarbage, (size_t)pGarbage->GetPPageIndex()->GetPPage());
						delete pGarbage;
					}
					LOG_FOOTER("SlotList::Free3(pSlotFree)", (size_t)pSlotFree);
					return false;
				}
			}
		}
		throw Exception((unsigned)IMemory::EException::_eFree);
	}

	// maintenance
	virtual void Show(const char* pTitle) {
		LOG_HEADER("SlotList::Show", pTitle, m_szSlot, (size_t)m_numSlots);
		Slot* pSlot = this->m_pHead;
		while (pSlot != nullptr) {
			LOG_NEWLINE("Slot-", (size_t)pSlot);
			pSlot = pSlot->pNext;
		}
		LOG_FOOTER("SlotList::Show", pTitle);
	};
};

