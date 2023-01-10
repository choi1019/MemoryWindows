#pragma once

#include "../typedef.h"
#define _MemoryVariable_Id _GET_CLASS_UID(_ELayer_Technical::_eMemoryEven)
#define _MemoryVariable_Name "MemoryEven"

#include "Memory.h"
#include "../../01Base/Object/BaseObject.h"
#include "../../01Base/Aspect/Exception.h"
#include "../../01Base/Aspect/Log.h"
#include "../../01Base/Aspect/Directory.h"

class SlotList: public BaseObject {
public:
	class Slot {
	private:
		// szRequested + szThis
		size_t m_szSize;
		Slot* m_pNext;
	public:
		size_t GetSize() { return this->m_szSize; }
		void SetSize(size_t szSize) { this->m_szSize = szSize; }

		Slot* GetNext() { return this->m_pNext; }
		void SetNext(Slot* pNext) { this->m_pNext = pNext; }

		void* GetMemory() { return (void*)(reinterpret_cast<size_t>(this) + sizeof(Slot)); }
		Slot* GetSlotFragment(size_t szRequested) {
			// start address of SlotFragment
			Slot* pSlotFragment = (Slot*)(reinterpret_cast<size_t>(this) + szRequested);
			// size of SlotFragment
			pSlotFragment->SetSize(this->m_szSize - szRequested);
			pSlotFragment->SetNext(nullptr);
			return pSlotFragment;
		}
	};

private:
	Slot* m_pHeadSlot;

	// working variable
	Slot* m_pCurrentSlot;
	Slot* m_pPreviousSlot;

	size_t m_uAvailable;

public:
	SlotList()
		: m_pHeadSlot(nullptr)
		, m_pPreviousSlot(nullptr)
		, m_pCurrentSlot(nullptr)
		, m_uAvailable(0)
	{
	}
	~SlotList() {
	}
	void InitializeMemory(void* pAllocated = nullptr, size_t szAllocated = 0) {
		BaseObject::Initialize();
		m_pHeadSlot = reinterpret_cast<Slot*>(pAllocated);
		if (pAllocated != nullptr) {
			m_pHeadSlot->SetSize(szAllocated);
			m_pHeadSlot->SetNext(nullptr);

			m_uAvailable = szAllocated;
		}
	}
	void FinalizeMemory() {
		m_pHeadSlot = nullptr;

		BaseObject::Finalize();
	}

	Slot* GetPHeadSlot() { return this->m_pHeadSlot; }

	size_t Show(const char* pTitle) {
		LOG_HEADER(pTitle);

		unsigned sum = 0;
		for (Slot* pSlot = m_pHeadSlot; pSlot != nullptr; pSlot = pSlot->GetNext()) {
			const char* pObjectName = "";
			Directory::Iterator itr = Directory::s_dirObjects.Find((long long)pSlot->GetMemory());
			if (itr != Directory::s_dirObjects.end()) {
				pObjectName = itr->second.c_str();
			}
			sum += static_cast<int>(pSlot->GetSize());
			LOG("Slot", String(pSlot->GetSize()) + "," + sum, pObjectName);
		}
		LOG_FOOTER(pTitle);
		return sum;
	}

	bool FindFittest(size_t szMemoryRequested) {
		m_pCurrentSlot = m_pHeadSlot;
		m_pPreviousSlot = nullptr;

		while (m_pCurrentSlot != nullptr) {
			if (szMemoryRequested <= m_pCurrentSlot->GetSize()) {
				return true;
			}
			m_pPreviousSlot = m_pCurrentSlot;
			m_pCurrentSlot = m_pCurrentSlot->GetNext();
		}
		return false;
	}

	Slot* RemoveFittest(size_t szMemoryRequested) {
		if (this->FindFittest(szMemoryRequested)) {
			// remove currentSlot
			if (m_pPreviousSlot == nullptr) {
				m_pHeadSlot = m_pCurrentSlot->GetNext();
			}
			else {
				m_pPreviousSlot->SetNext(m_pCurrentSlot->GetNext());
			}
			return m_pCurrentSlot;
		}
		return nullptr;
	}

	void InsertFittest(Slot* pSlotInsert) {
		this->FindFittest(pSlotInsert->GetSize());
		if (m_pPreviousSlot == nullptr) {
			pSlotInsert->SetNext(m_pCurrentSlot);
			this->m_pHeadSlot = pSlotInsert;
		}
		else {
			pSlotInsert->SetNext(m_pCurrentSlot);
			m_pPreviousSlot->SetNext(pSlotInsert);
		}
	}

	Slot* Merge(Slot* pSlot) {
		Slot* pSlotMerged = nullptr;
		Slot* pPrevious = nullptr;
		for (Slot* pCurrent = m_pHeadSlot; pCurrent != nullptr; pCurrent = pCurrent->GetNext()) {
			if (reinterpret_cast<size_t>(pCurrent) == reinterpret_cast<size_t>(pSlot) + pSlot->GetSize()) {
				// merge two slots
				if (pPrevious == nullptr) {
					m_pHeadSlot = pCurrent->GetNext();
				}
				else {
					pPrevious->SetNext(pCurrent->GetNext());
				}
				pSlot->SetSize(pSlot->GetSize() + pCurrent->GetSize());
				pSlotMerged = pSlot;
				break;
			}
			if (reinterpret_cast<size_t>(pCurrent) + pCurrent->GetSize() == reinterpret_cast<size_t>(pSlot)) {
				// merge two slots
				if (pPrevious == nullptr) {
					m_pHeadSlot = pCurrent->GetNext();
				}
				else {
					pPrevious->SetNext(pCurrent->GetNext());
				}
				pCurrent->SetSize(pCurrent->GetSize() + pSlot->GetSize());
				pSlotMerged = pCurrent;
				break;
			}
			pPrevious = pCurrent;
		}
		return pSlotMerged;
	}

	void Insert(Slot* pSlotInsert) {
		Slot *pSlotMerged = this->Merge(pSlotInsert);
		if (pSlotMerged == nullptr) {
			pSlotMerged = pSlotInsert;
		}
		this->InsertFittest(pSlotMerged);
	}

	Slot* Remove(Slot* pSlotRemove) {
		Slot* pPrevious = nullptr;
		for (Slot* pCurrent = m_pHeadSlot; pCurrent != nullptr; pCurrent = pCurrent->GetNext()) {
			if (pCurrent == pSlotRemove) {
				if (pPrevious == nullptr) {
					m_pHeadSlot = pCurrent->GetNext();
				}
				else {
					pPrevious->SetNext(pCurrent->GetNext());
				}
				return pCurrent;
			}
			pPrevious = pCurrent;
		}
		return nullptr;
	}

};

class MemoryVariable: public Memory
{
private:
	static unsigned s_uCountMemory;

	SlotList m_listFreeSlots;
	SlotList m_listUsedSlots;

public:
	MemoryVariable(
		size_t szAllocated,
		int nClassId = _MemoryVariable_Id,
		const char* typeName = _MemoryVariable_Name)
		: Memory(
			nClassId,
			typeName)
	{
		this->SetSzThis(*(size_t*)((long long)this - sizeof(size_t)));
		this->SetSzAllocated(szAllocated);
		this->SetPAllocated(nullptr);
	}
	virtual ~MemoryVariable() {
	}

	virtual void Initialize() {
		Memory::Initialize();
		this->SetPAllocated(Memory::s_pMemoryManager->SafeMalloc(this->GetSzAllocated(), this->GetClassName()));

		m_listFreeSlots.InitializeMemory(this->GetPAllocated(), this->GetSzAllocated());
		m_listUsedSlots.InitializeMemory();
	}
	virtual void Finalize() {
		m_listUsedSlots.FinalizeMemory();

		Memory::s_pMemoryManager->SafeFree(this->GetPAllocated());
		Memory::Finalize();
	}

	virtual size_t Show(const char* pTitle) {
		LOG_HEADER(pTitle, "this:", this->GetSzThis());

		size_t sumFreed = m_listFreeSlots.Show("FreeSlots");
		size_t sumUsed = m_listUsedSlots.Show("UsedSlots");

		size_t sumTotal = this->GetSzThis() + sumFreed + sumUsed;
		LOG_FOOTER(pTitle, "Sum:", sumTotal);

		return sumTotal;
	}

protected:

	void* Malloc(size_t szRequested, const char* pcName) {
		void* pAllocated = nullptr;
		size_t szAllocate = szRequested + sizeof(SlotList::Slot);
		SlotList::Slot *pSlotRemoved = this->m_listFreeSlots.RemoveFittest(szAllocate);

		if (pSlotRemoved == nullptr) {
			this->UnLock();
			throw Exception((unsigned)IMemory::EException::_eNoMoreSlot, GetClassName(), __func__, "_eNoMoreSlot");
		}
		else {
			// if size of requested memory is less than allocated, free the fragment
			if (pSlotRemoved->GetSize() >= szAllocate + sizeof(SlotList::Slot)) {
				SlotList::Slot* pSlotFragment = pSlotRemoved->GetSlotFragment(szAllocate);
				this->Free(pSlotFragment->GetMemory());
			}
			else {
				szAllocate = pSlotRemoved->GetSize();
			}

			// allocate a new memory
			pSlotRemoved->SetSize(szAllocate);
			pSlotRemoved->SetNext(nullptr);
			this->m_listUsedSlots.InsertFittest(pSlotRemoved);
			pAllocated = pSlotRemoved->GetMemory();
		}

		Directory::s_dirObjects[(long long)pAllocated] = pcName;
		return pAllocated;
	}

	void Free(void* pObject) {
		if (pObject == nullptr) {
			throw (Exception((int)IMemory::EException::_eNullPtr, GetClassName(), __func__), "_eNullPtr");
		}
		SlotList::Slot* pSlotFreed = (SlotList::Slot*)(reinterpret_cast<size_t>(pObject) - (sizeof(SlotList::Slot)));
		this->m_listUsedSlots.Remove(pSlotFreed);
		this->m_listFreeSlots.Insert(pSlotFreed);

		Directory::s_dirObjects.Remove((long long)pObject);
	}
};
