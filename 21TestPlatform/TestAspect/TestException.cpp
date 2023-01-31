#include <21TestPlatform/TestAspect/TestException.h>

unsigned TestException::s_uCount = 0;

TestException::TestException(
    unsigned nType,
    string sObject,
    string sFunction,
    string sMessage,
    unsigned nClassId,
    const char* pcClassName)

    : TestObject(nClassId, pcClassName)
    , m_nExceptionId(TestException::s_uCount++)
    , m_nType(nType)
    , m_sObject(sObject)
    , m_sFunction(sFunction)
    , m_sMessage(sMessage)
{
}

TestException::~TestException() {}

void TestException::Initialize() {
}
void TestException::Finalize() {
}

void TestException::Println() {
    printf(
        ">> TestException(%d) %s::%s %s\n"
        , m_nType
        , m_sObject.c_str()
        , m_sFunction.c_str()
        , m_sMessage.c_str()
        //			,String(m_tTime).c_str()
    );
}