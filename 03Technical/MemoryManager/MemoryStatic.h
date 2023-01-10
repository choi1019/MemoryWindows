#pragma once
#pragma warning(disable: 4291; disable: 4297)

#include "../typedef.h"
#define _MemoryStatic_Id _GET_CLASS_UID(_ELayer_Technical::_eMemoryStatic)
#define _MemoryStatic_Name "MemoryStatic"

#include "../../01Base/Aspect/Exception.h"
#include "../../01Base/Aspect/Log.h"
#include "../../01Base/Aspect/Directory.h"
#include "Memory.h"

class MemoryStatic : public Memory
{
public:
	void* operator new(size_t szThis, void* pAllocated)
	{
		*reinterpret_cast<size_t*>(pAllocated) = szThis;
		Memory::s_pMemoryManager = reinterpret_cast<IMemory *>((long long)pAllocated + sizeof(size_t));
		Directory::s_dirObjects[(long long)Memory::s_pMemoryManager] = "Memory::s_pMemoryManager";
		return Memory::s_pMemoryManager;
	}
	void operator delete(void* ptr)
	{
		Memory::s_pMemoryManager = nullptr;
		Directory::s_dirObjects.Remove((long long)ptr);
	}

private:
	void* m_pCurrentPtr;
	size_t m_szAvailable;

	void* Malloc(size_t szRequested, const char* pcName) {
		// if size of requested memory is less than available memory
		if (szRequested < sizeof(size_t)) {
			return nullptr;
		}
		if (szRequested > this->m_szAvailable) {
			this->UnLock();
			throw Exception((int)IMemory::EException::_eOutOfMemory, this->GetObjectId(), __func__, String(szRequested)+","+ m_szAvailable);
		}
		// store sizeThis
		*(size_t*)m_pCurrentPtr = szRequested;
		m_pCurrentPtr = reinterpret_cast<void*>((long long)m_pCurrentPtr + sizeof(size_t));
		this->m_szAvailable = this->m_szAvailable - sizeof(size_t);

		// allocate memory
		void* pAllocated = m_pCurrentPtr;
		m_pCurrentPtr = (void*)(reinterpret_cast<size_t>(m_pCurrentPtr) + szRequested);
		this->m_szAvailable = this->m_szAvailable - szRequested;

		Directory::s_dirObjects[(long long)pAllocated] = pcName;
		return pAllocated;
	}

	void Free(void* pObject)
	{
		if (pObject == nullptr) {
			throw Exception((int)IMemory::EException::_eNullPtr, this->GetObjectId(), __func__);
		}
		Directory::s_dirObjects.Remove((long long)pObject);
	}

public:
	MemoryStatic(
		// total memory allocated
		size_t szAllocated,
		int nClassId = _MemoryStatic_Id,
		const char* pcClassName = _MemoryStatic_Name)
		: Memory(nClassId, pcClassName)
		, m_szAvailable(0)
		, m_pCurrentPtr(nullptr)
	{
		this->SetSzThis(*(size_t*)((long long)this - sizeof(size_t)));
		this->SetSzAllocated(szAllocated - this->GetSzThis() - sizeof(size_t));
		this->SetPAllocated((void*)((long long)this + this->GetSzThis()));
	}
	virtual ~MemoryStatic()
	{
	}

	virtual void Initialize() {
		Memory::Initialize();

		this->m_szAvailable = this->GetSzAllocated();
		this->m_pCurrentPtr = this->GetPAllocated();
	}
	virtual void Finalize() {
		Memory::Finalize();
	}


	virtual size_t Show(const char* pTitle) {

		LOG_HEADER(pTitle, "Allocated, Available:", this->GetSzAllocated()+sizeof(size_t)+this->GetSzThis(), m_szAvailable);
		unsigned sum = 0;
		size_t szSize = 0;
		const char* pObjectName = "";
		for (void* pAllocated = reinterpret_cast<void*>((long long)this - sizeof(size_t));
			pAllocated < this->m_pCurrentPtr; 
			pAllocated = (void*)((long long)pAllocated + szSize))
		{
			pObjectName = "";
			Directory::Iterator itr = Directory::s_dirObjects.Find((long long)pAllocated + sizeof(size_t));
			if (itr != Directory::s_dirObjects.end()) {
				pObjectName = itr->second.c_str();
			}
			szSize = sizeof(size_t) + *reinterpret_cast<size_t*>(pAllocated);
			sum = sum+ static_cast<int>(szSize);
			LOG("Slot", szSize, pObjectName);
		}
		LOG_FOOTER(pTitle, "sum:", sum);

		return this->GetSzThis() + m_szAvailable;
	}
};
