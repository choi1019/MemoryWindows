#include <21TestPlatform/TestAspect/TestException.h>

unsigned TestException::s_uCount = 0;

TestException::TestException(
    int nType,
    string sObject,
    string sFunction,
    string sMessage,
    int nClassId,
    const char* pcClassName)

    : TestObject(nClassId, pcClassName)
    , m_nExceptionId(TestException::s_uCount++)
    , m_nType(nType)
    , m_sObject(sObject)
    , m_sFunction(sFunction)
    , m_sMessage(sMessage)
{
}