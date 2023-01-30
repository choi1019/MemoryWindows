#include <21TestPlatform/TestObject/TestObject.h>
#include <21TestPlatform/TestAspect/TestLog.h>

unsigned TestObject::s_uCounter = 0;
//size_t TestRoot::s_szThis = 0;

// static members
void* TestObject::operator new(size_t szThis, const char* sMessage) {
    void* pObject = malloc(szThis);
    printf("\n#TestRoot::malloc %s(%zu) = %zu", sMessage, szThis, (size_t)pObject);
    return pObject;
}
void TestObject::operator delete(void* pObject) {
    printf("\n#TestRoot::free (%zu)", (size_t)pObject);
    free(pObject);
}
void TestObject::operator delete(void* pObject, const char* sMessage) {
    printf("\n#DUMMY TestRoot::free %s(%zu)", sMessage,(size_t)pObject);
    free(pObject);
}

TestObject::TestObject(int nClassId, const char *pcClassName)
    : TestRoot(nClassId, pcClassName)
    , m_uObjectId(TestObject::s_uCounter++)
	, m_nClassId(nClassId)
	, m_pcClassName(pcClassName)
	, m_szThis(0)
{
}

TestObject::~TestObject() {}

void TestObject::BeforeInitialize() {
    TestLog().PrintSeparator();
    TestLog(this->GetClassName(), "Initialize", "���� �մϴ�").Println();
    TestLog().PrintSeparator();
    TestLog::AddTab();
}

void TestObject::Initialize() {
}

void TestObject::BeforeRun() {
    TestLog().PrintSeparator();
    TestLog(this->GetClassName(), "Run", "���� �մϴ�").Println();
    TestLog().PrintSeparator();
    TestLog::AddTab();
}

void TestObject::Run() {
}

void TestObject::AfterRun() {
    TestLog::RemoveTab();
    TestLog().PrintSeparator();
    TestLog(this->GetClassName(), "Run", "���� �Ǿ����ϴ�").Println();
    TestLog().PrintSeparator();
}

void TestObject::Finalize() {
}

void TestObject::AfterFinalize() {
    TestLog::RemoveTab();
    TestLog().PrintSeparator();
    TestLog(this->GetClassName(), "Finalize", "���� �Ǿ����ϴ�").Println();
    TestLog().PrintSeparator();
}