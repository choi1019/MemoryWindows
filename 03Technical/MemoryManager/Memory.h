#pragma once



#include "../../01Base/Memory/IMemory.h"
#include "PageManager.h"
#include "SlotManager.h"


#define SIZE_PAGE 256
#define SIZE_WORD 8

class Memory :public IMemory
{
private:
	// components
	PageManager* m_pPageManager;
	SlotManager* m_pSlotManager;

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
		, void* pMemoryAllocated
		, int nClassId = _Memory_Id
		, const char* pClassName = _Memory_Name)
	{
		LOG_HEADER("Memory::Memory");
		LOG((size_t)pMemoryAllocated, szMemoryAllocated, SIZE_PAGE, SIZE_WORD);
		this->m_pPageManager = new PageManager(SIZE_PAGE, szMemoryAllocated, pMemoryAllocated);
		this->m_pSlotManager = new SlotManager(SIZE_WORD, this->m_pPageManager);
		LOG_FOOTER("Memory");
	}
	virtual ~Memory() 
	{
	}
	virtual void Initialize() {
	}
	virtual void Finalize() {
	}

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
	virtual void Show(const char* pTitle) {
		LOG_HEADER("Memory::Show");
		this->m_pSlotManager->Show("");
		LOG_FOOTER("Memory");
	};
};
