#pragma once

#include "../typedef.h"
#include "../../01Base/Memory/IMemory.h"
#include "SystemMemoryObject.h"

#include "PageManager.h"
#include "SlotManager.h"

class Memory :public SystemMemoryObject, public IMemory
{
private:
	// components
	PageManager* m_pPageManager;
	SlotManager* m_pSlotManager;

protected:
	// critical section
	virtual void Lock() = 0;
	virtual void UnLock() = 0;

	virtual void* Malloc(size_t szObject, const char* pcName) {
		void* pObject = this->m_pSlotManager->Malloc(szObject);
		return pObject;
	}
	virtual void Free(void* pObject) {
		this->m_pSlotManager->Free(pObject);
	}

public:
	// constructors and destructors
	Memory(size_t szMemoryAllocated
		, size_t szPage
		, size_t szSlotUnit

		, int nClassId = _Memory_Id
		, const char* pClassName = _Memory_Name)
	{
		LOG_HEADER("Memory::Memory");
		LOG_NEWLINE(szPage, szSlotUnit);
		this->m_pPageManager = new("") PageManager(szMemoryAllocated, szPage);
		this->m_pSlotManager = new("") SlotManager(szSlotUnit);
		LOG_FOOTER("Memory");
	}
	virtual ~Memory() 
	{
		delete this->m_pPageManager;
		delete this->m_pSlotManager;
	}
	virtual void Initialize(void* pMemeoryAllocated) {
		SystemMemoryObject::Initialize();

		this->m_pPageManager->Initialize(pMemeoryAllocated);
		this->m_pSlotManager->Initialize(this->m_pPageManager);
	}
	virtual void Finalize() {
		this->m_pPageManager->Finalize();
		this->m_pSlotManager->Finalize();
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
		LOG_HEADER("Memory::Show-", pTitle);
		this->m_pSlotManager->Show("");
		LOG_FOOTER("Memory");
	};
};
