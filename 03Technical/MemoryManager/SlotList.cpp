#include <03Technical/MemoryManager/SlotList.h>

SlotList* SlotList::s_pSlotListFree = nullptr;

void* SlotList::operator new(size_t szThis, const char* sMessage) {
    if (MemoryObject::s_szSystemMemoryAllocated < szThis) {
        throw Exception((unsigned)IMemory::EException::_eNoMoreSystemMemory, "SlotList", "new", "_eNoMoreSystemMemory");
    }
    void* pNewSlotList = nullptr;
    if (SlotList::s_pSlotListFree == nullptr) {
        MemoryObject::s_szSystemMemoryAllocated -= szThis;
        pNewSlotList = MemoryObject::s_pCurrentSystemMemoryAllocated;
        MemoryObject::s_pCurrentSystemMemoryAllocated = (void*)((size_t)MemoryObject::s_pCurrentSystemMemoryAllocated + szThis);
        LOG_NEWLINE("***allocate SlotList");
    }
    else {
        pNewSlotList = SlotList::s_pSlotListFree;
        SlotList::s_pSlotListFree = SlotList::s_pSlotListFree->GetPNext();
        LOG_NEWLINE("***reuse SlotList");
    }
    LOG_NEWLINE("@new SlotList(szThis, pAllocated)", sMessage, szThis, (size_t)pNewSlotList);
    return pNewSlotList;
}

void SlotList::operator delete(void* pObject) {
    LOG_NEWLINE("@delete SlotList(pObject)", (size_t)pObject);
    SlotList* pSlotList = (SlotList*)pObject;
    pSlotList->SetPNext(SlotList::s_pSlotListFree);
    SlotList::s_pSlotListFree = pSlotList;
}

void SlotList::operator delete(void* pObject, const char* sMessage) {
    LOG_NEWLINE("@DUMMY delete SlotList(pObject)", sMessage, (size_t)pObject);
}