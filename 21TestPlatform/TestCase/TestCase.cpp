#include "21TestPlatform/TestCase/TestCase.h"

TestCase::TestCase(
    int nClassId,
    const char *pClassName)
    : TestObject(nClassId, pClassName)
{
}

TestCase::~TestCase() {}

void TestCase::InitializeCase() {
    this->Initialize();
}

void TestCase::FinalizeCase() {
    this->Finalize();
}

void TestCase::RunCase() {
    this->Run();
}

void TestCase::AssertNotNull(void* pObject) {
    if (pObject == nullptr) {
        throw TestException((unsigned)EAssertion::_eNotNull, this->GetClassName(), __func__, "Test Failed");
    }
}
void TestCase::AssertEqual(unsigned x, unsigned y) {
    if (x != y) {
        throw TestException((unsigned)EAssertion::_eEqual, this->GetClassName(), __func__, "Test Failed");
    }
}
void TestCase::AssertEqual(int x, int y) {
    if (x != y) {
        throw TestException((unsigned)EAssertion::_eEqual, this->GetClassName(), __func__, "Test Failed");
    }
}
void TestCase::AssertEqual(long long x, long long y) {
    if (x != y) {
        throw TestException((unsigned)EAssertion::_eEqual, this->GetClassName(), __func__, "Test Failed");
    }
}