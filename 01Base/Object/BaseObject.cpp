#include "BaseObject.h"

unsigned BaseObject::s_uCounter = 0;
IMemory* BaseObject::s_pMemory = nullptr;

void* BaseObject::operator new (size_t szThis, const char* sMessage) {
    printf("\n\n@BaseObject::new %s (%zu)", sMessage, szThis);
    void* pAllocated = BaseObject::s_pMemory->SafeMalloc(szThis, sMessage);
    return pAllocated;
}
void* BaseObject::operator new[] (size_t szThis, const char* sMessage) {
        return BaseObject::operator new(szThis, sMessage);
}
void BaseObject::operator delete(void* pObject) {
    printf("\n@BaseObject::delete %zu", (size_t)pObject);
    BaseObject::s_pMemory->SafeFree(pObject);
}
void BaseObject::operator delete[](void* pObject) {
    printf("\n@BaseObject::delete[] %zu", (size_t)pObject);
    BaseObject::s_pMemory->SafeFree(pObject);
}

// dummy
void BaseObject::operator delete(void* pObject, const char* sMessage) {
    printf("\n@DUMMY BaseObject::delete %zu", (size_t)pObject);
}
void BaseObject::operator delete[](void* pObject, const char* sMessage) {
    printf("\n@DUMMY BaseObject::delete[] %zu", (size_t)pObject);
}

BaseObject::BaseObject(int nClassId, const char* pcClassName)
    : RootObject(nClassId, pcClassName)
    , m_uObjectId(BaseObject::s_uCounter++)
    , m_nClassId(nClassId)
	, m_pcClassName(pcClassName)
	, m_eState(EState::eCreated)
	, m_szThis(0)
{
}
BaseObject::~BaseObject() {
    this->m_eState = EState::eDeleted;
}