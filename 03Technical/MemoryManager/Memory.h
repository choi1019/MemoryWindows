#pragma once

#include "../../01Base/Aspect/Exception.h"
#include "../../01Base/Object/BaseObject.h"
#include "../../01Base/Memory/IMemory.h"

#include <math.h>

#define SIZE_PAGE 1024

class Slot {
private:
	Slot* pNext;
};

class SlotIndex {
private:
	size_t m_idxPage;
	size_t m_szSlot;
	Slot* m_pHead;
	SlotIndex* m_pNext;

public:
	SlotIndex(size_t szSlot)
		: m_idxPage(0)
		, m_szSlot(szSlot)
		, m_pHead(nullptr)
		, m_pNext(nullptr)
	{
	}
	virtual ~SlotIndex() {
	}

	unsigned GetIdxPage() { return this->m_idxPage; }
	size_t GetSzSlot() { return this->m_szSlot; }
	SlotIndex* GetNext() { return this->m_pNext; }
	void SetNext(SlotIndex* pNext) { this->m_pNext = pNext;	}

	Slot* Malloc() { }
	void Free(void* pObject) {}
};

class SlotManager {
public:
	void* operator new(size_t szThis, void* pMemoryAllocated) {
		s_sizeThis = szThis;
		s_pMemoryAllocated = reinterpret_cast<void*>(reinterpret_cast<size_t>(pMemoryAllocated) + szThis);
		return s_pMemoryAllocated;
	}
	void operator delete(void* pObject) {
		s_pMemoryAllocated = nullptr;
	}

private:
	static void* s_pMemoryAllocated;
	static size_t s_sizeThis;

	size_t m_szWord;
	size_t m_szWordExponentOf2;
	size_t m_szPage;
	size_t m_szPageExponentOf2;

	size_t m_szAllocated;
	void* m_pMemoryAllocated;
	SlotIndex* m_pHeadSlotIndex;

public:
	SlotManager(size_t szAllocated) {
		// WORD size
		this->m_szWord = sizeof(size_t);
		this->m_szWordExponentOf2 = log2(this->m_szWord);
		// PAGE size
		this->m_szPage = SIZE_PAGE;
		this->m_szPageExponentOf2 = log2(this->m_szPage);

		this->m_pHeadSlotIndex = nullptr;
	}
	virtual ~SlotManager() {
	}

	void* Malloc(size_t szObject) {
		size_t szSlot = szObject;

		// multiple of WORD
		szSlot >> m_szWordExponentOf2;
		szSlot << m_szWordExponentOf2;
		szSlot += m_szWord;

		SlotIndex* pSlotIndex = this->m_pHeadSlotIndex;
		SlotIndex* pPreviousSlotIndex = this->m_pHeadSlotIndex;
		while (pSlotIndex != nullptr) {
			// if slotList is arleady generated
			if (pSlotIndex->GetSzSlot() == szSlot) {
				void *pSlotAllocated = pSlotIndex->Malloc();
				break;
			} 
			pPreviousSlotIndex = pSlotIndex;
			pSlotIndex = pSlotIndex->GetNext();
		}
		// if slotlist of the size is not generated
		if (m_pHeadSlotIndex == nullptr) {
			pSlotIndex = new SlotIndex(szSlot);
			this->m_pHeadSlotIndex = pSlotIndex;
		}
		else {
			pSlotIndex = new SlotIndex(szSlot);
			pSlotIndex->SetNext(pPreviousSlotIndex->GetNext());
			pPreviousSlotIndex->SetNext(pSlotIndex);
		}
		return pSlotIndex->Malloc();
	}
	void Free(void* pObject) {
		size_t idxPage = reinterpret_cast<unsigned>(pObject) >> this->m_szPageExponentOf2;
		SlotIndex* pSlotIndex = this->m_pHeadSlotIndex;
		while (pSlotIndex != nullptr) {
			if (pSlotIndex->GetIdxPage() == idxPage) {
				pSlotIndex->Free(pObject);
				break;
			}
			pSlotIndex = pSlotIndex->GetNext();
		}
		throw Exception(static_cast<unsigned>(IMemory::EException::_eNullPtr));
	}
};

class Memory :public IMemory, public BaseObject
{
public:
	void* operator new(size_t szThis, void* pMemoryAllocated) {
		s_sizeThis = szThis;
		s_pMemoryAllocated = reinterpret_cast<void*>(reinterpret_cast<size_t>(pMemoryAllocated) + szThis);
		return s_pMemoryAllocated;
	}
	void operator delete(void* pObject) {
		s_pMemoryAllocated = nullptr;
	}
	//  matching operator - new(size_t szThis, const char* pcName="")
	void operator delete(void* pObject, char* pMemoryAllocated) {
		s_pMemoryAllocated = nullptr;
	}

private:
	static void* s_pMemoryAllocated;
	static size_t s_sizeThis; 

	// attributes
	SlotManager* m_pSlotManager;

	size_t m_szMemoryAllocated;
	void* m_pMemoryAllocated;

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
		, int nClassId = _Memory_Id
		, const char* pClassName = _Memory_Name)
		: BaseObject(nClassId, pClassName)
		, m_szMemoryAllocated(szMemoryAllocated)
		, m_pMemoryAllocated(s_pMemoryAllocated)
	{
//		this->m_pPageManager = new PageManager();
		this->m_pSlotManager = new(s_pMemoryAllocated) SlotManager(szMemoryAllocated);
	}
	virtual ~Memory() 
	{
	}
	virtual void Initialize() {
	}
	virtual void Finalize() {
	}

	size_t GetSzAllocated() { return this->m_szMemoryAllocated; }
	void SetSzAllocated(size_t sizeAllocated) { this->m_szMemoryAllocated = sizeAllocated; }

	void* GetPAllocated() { return this->m_pMemoryAllocated; }
	void SetPAllocated(void* pAllocated) { this->m_pMemoryAllocated = pAllocated; }

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
	virtual size_t Show(const char* pTitle) {};
};

#define SHOW_MEMORYSTATIC(MESSAGE) MemoryStatic::s_pMemoryManager->Show(MESSAGE)
#define SHOW_MEMORYEVEN(CLASS, MESSAGE) CLASS::s_pMemory->Show(MESSAGE)
#define SHOW_MEMORYVARIABLE(MESSAGE) BaseObject::s_pMemory->Show(MESSAGE)
