
#include <21TestPlatform/TestCase/TestSuite.h>
#include <21TestPlatform/TestAspect/TestException.h>

TestSuite::TestSuite(unsigned nClassId, const char* pClassName)
    : TestObject(nClassId, pClassName)
    , m_uLength(0)
    , m_uCurrentIndex(0)
    , m_vPTestObjects()
{
}
TestSuite::~TestSuite() 
{
}

void TestSuite::add(TestObject* pTestCase) {
    this->m_vPTestObjects.push_back(pTestCase);
}
void TestSuite::DeleteTestCases() {
    for (TestObject* pTestCase : m_vPTestObjects) {
        delete pTestCase;
    }
}

void TestSuite::InitializeSuite() {
    this->Initialize();
}
void TestSuite::FinalizeSuite() {
    this->DeleteTestCases();
    this->Finalize();
}
void TestSuite::RunSuite() {
    try {
        for (TestObject* pTestCase: m_vPTestObjects) {
            pTestCase->BeforeInitialize();
            pTestCase->Initialize();
            pTestCase->BeforeRun();
            pTestCase->Run();
            pTestCase->AfterRun();
            pTestCase->Finalize();
            pTestCase->AfterFinalize();
        }
        this->Run();
    }
    catch (TestException& exception) {
        exception.Println();
    }
}