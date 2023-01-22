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
	static SlotList* s_pSlotListFree;
//	static SlotList* s_pSlotListInUse;

public:
	void* operator new(size_t szThis, const char* sMessage) {
		LOG_NEWLINE("@new SlotList(sMessage, szThis)", sMessage, szThis);

		void* pNewSlotList = nullptr;
		if (s_pSlotListFree == nullptr) {
			s_szSystemMemoryAllocated -= szThis;
			pNewSlotList = s_pCurrentSystemMemoryAllocated;
			s_pCurrentSystemMemoryAllocated = (void*)((size_t)s_pCurrentSystemMemoryAllocated + szThis);			
		}
		else {
			pNewSlotList = s_pSlotListFree;
			s_pSlotListFree = s_pSlotListFree->GetPNext();
		}
		///////////////////////////////////////////////
		///////////////////////////////////////////////
		return pNewSlotList;
	}
	void operator delete(void* pObject) {
		LOG_NEWLINE("@delete SlotList(pObject)", (size_t)pObject);
		SlotList* pSlotList = (SlotList*) pObject;
		pSlotList->SetPNext(s_pSlotListFree);
		s_pSlotListFree = pSlotList;
		///////////////////////////////////////////////
		
		///////////////////////////////////////////////
	}
	void operator delete(void* pObject, const char* sMessage) {
		SlotList* pSlotList = (SlotList*)pObject;
		pSlotList->SetPNext(s_pSlotListFree);
		s_pSlotListFree = pSlotList;
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
	// getters and setters
	PageIndex* GetPPageIndex() { return this->m_pPageIndex; }
	size_t GetSzSlot() { return this->m_szSlot; }
	SlotList* GetPNext() { return this->m_pNext; }
	void SetPNext(SlotList* pNext) { this->m_pNext = pNext; }
	SlotList* GetPSibling() { return this->m_pSibling; }
	void SetPSibling(SlotList* pSibling) { this->m_pSibling = pSibling; }
	bool IsGarbage() { return this->m_bGarbage; }

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

		// compute the number of pages required
		size_t szPage = m_pPageList->GetSzPage();
		size_t numPagesRequired = m_szSlot / szPage;
		if (m_szSlot > numPagesRequired * szPage) {
			numPagesRequired++;
		}
		// get required number of pages
		this->m_pPageIndex = m_pPageList->Malloc(numPagesRequired);
		Page* pPage = this->m_pPageIndex->GetPPage();
		this->m_index = this->m_pPageIndex->GetIndex();
		LOG_NEWLINE("m_pPageList::m_pPageList(pPageFirst,pPageLast)", (size_t)pPage
			, (size_t)m_pPageIndex->GetPPageIndexLast()->GetPPage());

		// compute the number of slots allocatable
		this->m_numMaxSlots = numPagesRequired * szPage / m_szSlot;
		this->m_numSlots = this->m_numMaxSlots;


		LOG_HEADER("SlotList::Slots(m_numSlots, m_pHead)", m_numSlots, (size_t)pPage);
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
		LOG_FOOTER("SlotList::Slots");

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

	Slot* Malloc(size_t szObject, SlotList *pPrevious) {
		// if the SlotList of the same size is found
		LOG_HEADER("SlotList::Malloc(szObject, pPrevious)", szObject, (size_t)pPrevious);
		if (m_szSlot == szObject) {
			LOG_NEWLINE("m_szSlot == szObject", "found");
			if (this->m_pHead == nullptr) {
				// if all the slots are in use
				if (this->m_pSibling == nullptr) {
					// generate a new Sibling SlotList
					this->m_pSibling = new("SlotList::m_pSibling") SlotList(szObject, m_pPageList);
				}
				Slot* pSlot = this->m_pSibling->Malloc(szObject, nullptr);
				LOG_FOOTER("SlotList::Malloc Sibling(pSlot, m_numSlots)", (size_t)pSlot, m_numSlots);
				return pSlot;

			} else {
				// allocate a slot at m_pHead
				Slot* pSlot = this->m_pHead;
				this->m_pHead = this->m_pHead->pNext;
				this->m_numSlots--;
				LOG_FOOTER("SlotList::Malloc Head(pSlot, m_numSlots)", (size_t)pSlot, m_numSlots);
				return pSlot;
			}
		}
		// if the slot size of current SlotList is less than needed, 
		// proceed to the next SlotList
		else if (m_szSlot < szObject) {
			if (m_pNext == nullptr) {
				LOG_NEWLINE("m_szSlot < szObject", "found");
				// generate new SlotList of the same size - Sibling
				this->m_pNext = new("SlotList::m_pNext") SlotList(szObject, m_pPageList);
			}
			Slot* pSlot = this->m_pNext->Malloc(szObject, this);
			LOG_FOOTER("SlotList::Malloc Next(pSlot, m_numSlots)", (size_t)pSlot, m_numSlots);
			return pSlot;
		}
		// if the slot size of current SlotList is grated than the required size,
		// generate new SlotList
		else {
			LOG_NEWLINE("m_szSlot > szObject", "found");
			SlotList* pNewSlotList = new("") SlotList(m_szSlot, m_pPageList);
			// insert a new slot
			pNewSlotList->SetPNext(this);
			pPrevious->SetPNext(pNewSlotList);
			Slot* pSlot = pNewSlotList->Malloc(szObject, this);
			LOG_FOOTER("SlotList::Malloc Insert(pSlot, m_numSlots)", (size_t)pSlot, m_numSlots);
			return pSlot;
		}
	}
	bool Free(Slot* pSlotFree, size_t targetIndex) {
		// found
		LOG_HEADER("SlotList::Free(pSlotFree, indexPage)", (size_t)pSlotFree, targetIndex);
		if (targetIndex == this->m_index) {
			LOG_NEWLINE("targetIndex == this->m_index");
			// insert pSlotFree to Slot LIst
			pSlotFree->pNext = m_pHead;
			m_pHead = pSlotFree;
			this->m_numSlots++;
			if (m_numSlots == m_numMaxSlots) {
				// this is garbage
				this->m_bGarbage = true;
			}
			LOG_FOOTER("SlotList::Free-Found(pSlotFree)", (size_t)pSlotFree);
			return true;
		}
		else {
			LOG_NEWLINE("targetIndex != this->m_index");
			// search in the sibling list
			if (this->m_pSibling != nullptr) {
				LOG_NEWLINE("this->m_pSibling != nullptr");
				bool found = this->m_pSibling->Free(pSlotFree, targetIndex);
				if (found) {
					if (this->m_pSibling->IsGarbage()) {
						SlotList* pGarbage = this->m_pSibling;
						this->m_pSibling = m_pSibling->GetPSibling();
						delete pGarbage;
					}
					LOG_FOOTER("SlotList::Free-Found Sibling(pSlotFree)", (size_t)pSlotFree);
					return found;
				}
			}
			// search in the next list
			if (this->m_pNext != nullptr) {
				LOG_NEWLINE("this->m_pNext != nullptr");
				bool found = this->m_pNext->Free(pSlotFree, targetIndex);
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
					LOG_FOOTER("SlotList::Free-next Found(pSlotFree)", (size_t)pSlotFree);
					return found;
				}
			}
			LOG_FOOTER("SlotList::Free-False(pSlotFree)", (size_t)pSlotFree);
			return false;
		}
//		throw Exception((unsigned)IMemory::EException::_eFree, "SlotList", "Free", "notFound");
	}

	// maintenance
	virtual void Show(const char* pTitle) {
		LOG_HEADER("SlotList::Show(m_szSlot,Index)", pTitle, m_szSlot, (size_t)GetPPageIndex()->GetIndex());
		Slot* pSlot = this->m_pHead;
		while (pSlot != nullptr) {
			LOG_NEWLINE("Slot-", (size_t)pSlot);
			pSlot = pSlot->pNext;
		}
		LOG_FOOTER("SlotList::Show", pTitle);
	};
};

