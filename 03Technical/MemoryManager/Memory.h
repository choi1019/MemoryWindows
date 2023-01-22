#pragma once

#include <stdlib.h>
#include "../typedef.h"
#include "../../01Base/Memory/IMemory.h"
#include "SystemMemoryObject.h"

#include "PageList.h"
#include "SlotList.h"

class Memory :public SystemMemoryObject, public IMemory
{
public:
	void* operator new(size_t szThis, const size_t szSystemMemory, void* pSystemMemory) {
		LOG_NEWLINE("@new Memory(szThis,szSystemMemory,pSystemMemory)"
			, szThis, szSystemMemory, (size_t)pSystemMemory);
		s_pSystemMemoryAllocated = pSystemMemory;
		s_pCurrentSystemMemoryAllocated = (void *)((size_t)pSystemMemory + szThis);
		s_szSystemMemoryAllocated = szSystemMemory - szThis;
		return s_pSystemMemoryAllocated;
	}
	void operator delete(void* pObject) {
		LOG_NEWLINE("@delete Memory(pObject)", (size_t)pObject);
	}
	void operator delete(void* pObject, const size_t szSystemMemory, void* pSystemMemory) {
	}

private:
	size_t m_szUnit;
	size_t m_szPage;

	SlotList* m_pHead;
	SlotList* m_pFreeHead;
	PageList* m_pPageList;

	size_t m_szUnitExponentOf2;
	size_t m_szPageExponentOf2;

protected:
	// critical section
	virtual void Lock() = 0;
	virtual void UnLock() = 0;

	void* Malloc(size_t szObject, const char* pcName = "") { 
		size_t szSlot = szObject;

		// multiple of WORD
		szSlot >>= m_szUnitExponentOf2;
		szSlot <<= m_szUnitExponentOf2;
		if (szSlot < szObject) {
			szSlot += m_szUnit;
		}

		LOG_HEADER("Memory::Malloc(szObject, szSlot)", szObject, szSlot);
		if (m_pHead == nullptr) {
			// if any SlotList is not generated
			LOG_NEWLINE("m_pHead == nullptr");
			m_pHead = new("SlotList") SlotList(szSlot, m_pPageList);
		}
		else {
			LOG_NEWLINE("m_pHead != nullptr");
		}

		Slot* pSlot = m_pHead->Malloc(szObject, nullptr);
		LOG_FOOTER("Memory::Malloc(pSlot)", (size_t)pSlot);
		return pSlot;
	}

	void Free(void* pObject) {
		LOG_HEADER("Memory::Free", (size_t)pObject);
		size_t indexPObject = (size_t)pObject >> m_szPageExponentOf2;
		bool found = this->m_pHead->Free((Slot*)pObject, indexPObject);
		// if m_pHead is a target SlotList
		if (found) {
			LOG_NEWLINE("found");
			// if m_pHead is a Garbage
			if (this->m_pHead->IsGarbage()) {
				LOG_NEWLINE("if (this->m_pHead->IsGarbage())");
				SlotList* pGarbage = this->m_pHead;
				if (this->m_pHead->GetPSibling() != nullptr) {
					LOG_NEWLINE("if (this->m_pHead->GetPSibling() != nullptr)");
					// promote pSibling and delete pGarbage
					this->m_pHead = pGarbage->GetPSibling();
					this->m_pHead->SetPNext(pGarbage->GetPNext());
				}
				else {
					LOG_NEWLINE("if (this->m_pHead->GetPSibling() == nullptr)");
					this->m_pHead = pGarbage->GetPNext();
				}
				LOG_NEWLINE("delete pGarbage", (size_t)pGarbage);
				delete pGarbage;
			}
		}
		else {
			throw Exception((unsigned)IMemory::EException::_eFree, "Memory", "Free"
				, (size_t)pObject);
			exit(1);
		}
		LOG_FOOTER("Memory::Free");
	}

public:
	// constructors and destructors
	Memory(void* pMemeoryAllocated
		, size_t szMemoryAllocated
		, size_t szPage
		, size_t szSlotUnit
		, int nClassId = _Memory_Id
		, const char* pClassName = _Memory_Name)
	{
		this->m_szPage = szPage;
		this->m_szUnit = szSlotUnit;

		LOG_HEADER("Memory::Memory");
		this->m_pPageList = new("PageList") PageList((size_t)pMemeoryAllocated, szMemoryAllocated, m_szPage);
		this->m_pHead = nullptr;
		this->m_pFreeHead = nullptr;
		this->m_szUnitExponentOf2 = (size_t)(log2(static_cast<double>(this->m_szUnit)));
		this->m_szPageExponentOf2 = (size_t)(log2(static_cast<double>(this->m_szPage)));

		LOG_FOOTER("Memory");
	}
	virtual ~Memory() 
	{
		this->m_pPageList->Finalize();
		delete this->m_pPageList;
	}

	virtual void Initialize() {
		LOG_HEADER("Memory::Initialize");
		SystemMemoryObject::Initialize();
		LOG_FOOTER("Memory::Initialize");
	}
	virtual void Finalize() {
		LOG_HEADER("Memory::Finalize");
		SystemMemoryObject::Finalize();
		LOG_FOOTER("Memory::Finalize");
	}

	// methods
	void* SafeMalloc(size_t szAllocate, const char* pcName = "")
	{
		try {
			Lock();
			void* pMemoryAllocated = this->Malloc(szAllocate, pcName);
			UnLock();
			return pMemoryAllocated;
		}
		catch (Exception& exception) {
			exception.Println();
			exit(1);
		}
	}
	void SafeFree(void* pObject) {
		try {
			Lock();
			this->Free(pObject);
			UnLock();
		}
		catch (Exception& exception) {
			exception.Println();
			exit(1);
		}
	}

	// maintenance
	virtual void Show(const char* pTitle) {
		LOG_HEADER("Memory::Show-", pTitle);
		LOG_NEWLINE("SystemMemory(size, current, allocated)"
			, SystemMemoryObject::s_szSystemMemoryAllocated
			, (size_t)SystemMemoryObject::s_pCurrentSystemMemoryAllocated
			, (size_t)SystemMemoryObject::s_pSystemMemoryAllocated
		);
		m_pPageList->Show("");

		SlotList* pSlotList = this->m_pHead;
		while (pSlotList != nullptr) {
			pSlotList->Show("Next");
			SlotList* pSiblingSlotList = pSlotList->GetPSibling();
			while (pSiblingSlotList != nullptr) {
				pSiblingSlotList->Show("Sibling");
				pSiblingSlotList = pSiblingSlotList->GetPSibling();
			}
			pSlotList = pSlotList->GetPNext();
		}
		LOG_FOOTER("Memory::Show");
	};
};
