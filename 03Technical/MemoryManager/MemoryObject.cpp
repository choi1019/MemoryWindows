#include <03Technical/MemoryManager/MemoryObject.h>

void* MemoryObject::s_pSystemMemoryAllocated = nullptr;
void* MemoryObject::s_pCurrentSystemMemoryAllocated = nullptr;
size_t MemoryObject::s_szSystemMemoryAllocated = 0;