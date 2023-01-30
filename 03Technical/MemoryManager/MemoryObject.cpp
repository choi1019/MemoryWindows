#include <03Technical/MemoryManager/MemoryObject.h>

void* MemoryObject::s_pSystemMemoryAllocated = nullptr;
void* MemoryObject::s_pCurrentSystemMemoryAllocated = nullptr;
size_t MemoryObject::s_szSystemMemoryAllocated = 0;

void* MemoryObject::operator new(size_t szThis, const char *sMessage) {
    if (MemoryObject::s_szSystemMemoryAllocated < szThis) {
        throw Exception((unsigned)IMemory::EException::_eNoMoreSystemMemory, sMessage, "new", "_eNoMoreSystemMemory");
    }
    MemoryObject::s_szSystemMemoryAllocated -= szThis;
    void* pCurrentSystemMemoryAllocated = MemoryObject::s_pCurrentSystemMemoryAllocated;
    MemoryObject::s_pCurrentSystemMemoryAllocated = (void*)((size_t)MemoryObject::s_pCurrentSystemMemoryAllocated + szThis);
    LOG_NEWLINE("@new ", sMessage, "(szThis, pAllocated)", szThis, (size_t)pCurrentSystemMemoryAllocated);
    return pCurrentSystemMemoryAllocated;
}
void* MemoryObject::operator new[](size_t szThis, const char* sMessage) {
    return MemoryObject::operator new(szThis, sMessage);
}
void MemoryObject::operator delete(void* pObject) {
    LOG_NEWLINE("@delete SystemMemoryObject(pObject)", (size_t)pObject);
}
void MemoryObject::operator delete[](void* pObject) {
    LOG_NEWLINE("@delete[] SystemMemoryObject(pObject)", (size_t)pObject);
}

void MemoryObject::operator delete(void* pObject, const char* sMessage) {
}
void MemoryObject::operator delete[](void* pObject, const char* sMessage) {
}