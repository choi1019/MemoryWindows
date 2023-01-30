#include <03Technical/MemoryManager/Memory.h>

	void* Memory::operator new(size_t szThis, void* pSystemMemory, const size_t szSystemMemory) {
		LOG_NEWLINE("@new Memory(szThis,pSystemMemory,szSystemMemory)"
			, szThis, (size_t)pSystemMemory, szSystemMemory);
		if (szSystemMemory < szThis) {
			throw Exception((unsigned)IMemory::EException::_eNoMoreSystemMemory, "Memory","new","_eNoMoreSystemMemory");
		}		
		
		MemoryObject::s_pSystemMemoryAllocated = pSystemMemory;
		MemoryObject::s_pCurrentSystemMemoryAllocated = (void *)((size_t)pSystemMemory + szThis);
		MemoryObject::s_szSystemMemoryAllocated = szSystemMemory - szThis;

		SlotList::s_pSlotListFree = nullptr;

		return MemoryObject::s_pSystemMemoryAllocated;
	}
	void Memory::operator delete(void* pObject) {
		LOG_NEWLINE("@delete Memory(pObject)", (size_t)pObject);
	}
	void Memory::operator delete(void* pObject, void* pSystemMemory, const size_t szSystemMemory) {
	}
