#include "21TestPlatform/TestCase/TestCase.h"
#include <21TestPlatform/TestAspect/TestException.h>

TestCase::TestCase(
    unsigned nClassId,
    const char *pClassName)
    : TestObject(nClassId, pClassName)
{
}

TestCase::~TestCase() {
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