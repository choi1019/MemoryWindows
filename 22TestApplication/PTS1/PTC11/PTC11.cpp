
#include <22TestApplication/PTS1/PTC11/PTC11.h>

// #include <13PTechnical/PMemoryManager/PMemory.h>
// #include <22TestApplication/PTS1/PTC11/DomainObject11.h>

PTC11::PTC11(unsigned nClassId, const char* pcClassName)
    : TestCase(nClassId, pcClassName)
//    , m_pDomainObject(nullptr)
{
    // this->m_pDomainObject = new("PTC11::DomainObject") DomainObject11();
    // BaseObject::s_pMemory->Show("");
}
PTC11::~PTC11() {
    // delete this->m_pDomainObject;
    // BaseObject::s_pMemory->Show("delete PTC11::m_pDomainObject");
}

void PTC11::Initialize() {
}
void PTC11::Finalize() {
}
void PTC11::Run() {
    // test case
    // this->m_pDomainObject->Run();
}