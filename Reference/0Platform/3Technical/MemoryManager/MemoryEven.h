#pragma once

/////////////////////////////////////////////////////////////////////////
// https://developer.ibm.com/technologies/systems/tutorials/au-memorymanager/
// http://www2.lawrence.edu/fast/GREGGJ/CMSC270/memory_manager.html
/////////////////////////////////////////////////////////////////////////

#include "../typedef.h"
#define _MemoryEven_Id _GET_CLASS_UID(_ELayer_Technical::_eMemoryEven)
#define _MemoryEven_Name "MemoryEven"

#include "MemoryDynamic.h"
#include "../../1Base/Aspect/Exception.h"
#include "../../1Base/Aspect/Log.h"

#undef GetClassName

class MemoryEven: public MemoryDynamic
{
private:
	class Slot {
	private:
		Slot* m_pNext = nullptr;
	public:
		Slot* GetNext() { return this->m_pNext; }
		void SetNext(Slot* pNext) { this->m_pNext = pNext; }
	};

	size_t m_szSlot;
	size_t m_uCountTotalSlots;
	Slot* m_pHeadSlot;

public:
	MemoryEven(
		size_t szSlot,
		size_t uCountTotalSlots,
		int nClassId = _MemoryEven_Id,
		const char* pClassName = _MemoryEven_Name)
		: MemoryDynamic(
			size_t(szSlot* uCountTotalSlots),
			nClassId,
			pClassName)
		, m_szSlot(szSlot)
		, m_uCountTotalSlots(uCountTotalSlots)
		, m_pHeadSlot(nullptr)
	{
	}

	virtual ~MemoryEven() {
	}

	virtual void InitializeMemory() {
		MemoryDynamic::InitializeMemory();

		// if requested size of a slot is smaller than size of class Slot
		if (this->m_szSlot < sizeof(Slot)) {
			throw (Exception((unsigned)IMemory::EError::_eSlotSizeSmall, this->GetObjectId(), __func__, "_eSizeSlotIsTooSmall"));
		}
		if (m_uCountTotalSlots == 0) {
			this->m_pHeadSlot = nullptr;
			throw Exception(
				(int)IMemory::EError::_eSlotCountZero,
				this->GetObjectId(),
				__func__,
				"_eSlotCountIsZero");
		}

		m_pHeadSlot = reinterpret_cast<Slot*>(this->GetPAllocated());
		Slot* pCurrentSlot = m_pHeadSlot;
		for (unsigned i = 0; i < m_uCountTotalSlots - 1; i++)
		{
			pCurrentSlot->SetNext((Slot*)(reinterpret_cast<size_t>(pCurrentSlot) + m_szSlot));
			pCurrentSlot = (Slot*)(reinterpret_cast<size_t>(pCurrentSlot) + m_szSlot);
		}
		pCurrentSlot->SetNext(nullptr);
	}

	virtual void FinalizeMemory() {
		MemoryDynamic::FinalizeMemory();
	}

	virtual size_t Show(const char *pMessage) {
		LOG_HEADER(pMessage);
		LOG("SizeThis, SizeSlot, CountSlots", this->GetSzThis(), m_szSlot, m_uCountTotalSlots);
		unsigned index = 0;
		for (Slot* pSlot = m_pHeadSlot; pSlot != nullptr; pSlot = pSlot->GetNext()) {
			index++;
		}
		LOG_FOOTER(String(pMessage) + "- Available:" + index);

		return this->GetSzThis() + (m_szSlot * m_uCountTotalSlots);
	}

protected:
	virtual void* Malloc(size_t szSize, const char* pcName) {
		if (nullptr == m_pHeadSlot) {
			this->UnLock();
			throw Exception(
				(int)IMemory::EError::_eNoMoreSlot, 
				GetClassName(), 
				__func__, 
				"_eNoMoreSlot"
			);
		}
		Slot* pAllocatedSlot = this->m_pHeadSlot;
		this->m_pHeadSlot = pAllocatedSlot->GetNext();

		return pAllocatedSlot;
	}
	virtual void Free(void* pObject) {
		if (pObject == nullptr) {
			this->UnLock();
			throw Exception(
				(int)IMemory::EError::_eNullPtr,
				GetClassName(), 
				__func__, 
				"_eNullPtr"
			);
		}
		Slot* pSlotDelete = reinterpret_cast<Slot*>(pObject);
		pSlotDelete->SetNext(this->m_pHeadSlot);
		this->m_pHeadSlot = pSlotDelete;
	}
};
