#pragma once

#include "../typedef.h"
#include "SystemMemoryObject.h"
#include "PageManager.h"
#include "SlotList.h"

class SlotManager : public SystemMemoryObject {

private:
	PageManager* m_pPageManager;

	size_t m_szWord;
	size_t m_szPage;

	SlotList* m_pHead;
	SlotList* m_pFreeHead;

public:
	SlotManager(size_t szWord) {
		LOG_HEADER("SlotManager::SlotManager", szWord);

		// WORD size
		this->m_szWord = szWord;
		// PAGE size
		this->m_szPage = 0;
		this->m_pHead = nullptr;
		this->m_pFreeHead = nullptr;
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
		// if any SlotList is not generated
		if (m_pHead == nullptr) {
			LOG_NEWLINE("m_pHead == nullptr");
			m_pHead = new("") SlotList(szSlot);
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
		// if m_pHead is a target SlotList
		if (found) {
			LOG_NEWLINE("found");
			// if m_pHead is a Garbage
			if (this->m_pHead->IsGarbage()) {
				LOG_NEWLINE("if (this->m_pHead->IsGarbage())");
				SlotList* pGarbage = this->m_pHead;
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
		SlotList* pSlotList = this->m_pHead;
		while (pSlotList != nullptr) {
			pSlotList->Show("Next");
			SlotList *pSiblingSlotList = pSlotList->GetPSibling();
			while (pSiblingSlotList != nullptr) {
				pSiblingSlotList->Show("Sibling");
				pSiblingSlotList = pSiblingSlotList->GetPSibling();
			}
			pSlotList = pSlotList->GetPNext();
		}
		LOG_FOOTER("SlotManager");
	}
};

