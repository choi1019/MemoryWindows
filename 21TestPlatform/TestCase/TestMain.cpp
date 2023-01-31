#include <21TestPlatform/TestCase/TestMain.h>
#include <21TestPlatform/TestAspect/TestException.h>

TestMain::TestMain(unsigned nClassId, const char* pClassName)
    : TestSuite(nClassId, pClassName)
{
}
TestMain::~TestMain() {
}

void TestMain::InitializeMain() {
    TestObject::Initialize();
}
void TestMain::FinalizeMain() {
    TestObject::Finalize();
}

void TestMain::RunMain() {
    try {
        for (TestObject* pTestObject : m_vPTestObjects) {
            TestSuite *pTestSuite = (TestSuite *)pTestObject;
            pTestSuite->BeforeInitialize();
            pTestSuite->InitializeSuite();
            pTestSuite->BeforeRun();
            pTestSuite->RunSuite();
            pTestSuite->AfterRun();
            pTestSuite->FinalizeSuite();
            pTestSuite->AfterFinalize();
        }
        this->Run();
    }
    catch (TestException& exception) {
        exception.Println();
    }
}
