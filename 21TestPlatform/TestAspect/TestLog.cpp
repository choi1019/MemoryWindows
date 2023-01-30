#include <21TestPlatform/TestAspect/TestLog.h>

unsigned TestLog::s_uCount = 0;

TestLog::TestLog(
    string sObject,
    string sFunction,
    string sMessage,
    unsigned nClassId, 
    const char* pcClassName)
    : TestObject(nClassId, pcClassName)
    , m_nLogId(TestLog::s_uCount++)
    , m_sObject(sObject)
    , m_sFunction(sFunction)
    , m_sMessage(sMessage)
{
}

TestLog::TestLog (unsigned nClassId , const char* pcClassName)
    : TestObject(nClassId, pcClassName)
    , m_nLogId(TestLog::s_uCount++)
{
}

TestLog::~TestLog() {
}
void TestLog::Initialize() {
}
void TestLog::Finalize() {
}

void TestLog::Println() {
    printf(
        "\n%sTestLog%d %s::%s %s"
        , TestLog::GetTab()
        , this->GetLogId()
        , m_sObject.c_str()
        , m_sFunction.c_str()
        , m_sMessage.c_str()
        //			,String(m_tTime).c_str()
    );
}
void TestLog::PrintSeparator() {
    printf("\n%s-------------------------------------------------", this->GetTab());
}


#define COUNT_TAB 20
#define SIZE_TAB 4
#define SPACE ' '

unsigned TestLog::s_uCountTab = 0;
char TestLog::s_pcTab[COUNT_TAB * SIZE_TAB] = {};

char* TestLog::GetTab() {
    for (unsigned i = 0; i < TestLog::s_uCountTab * SIZE_TAB; i++) {
        TestLog::s_pcTab[i] = SPACE;
    }
    TestLog::s_pcTab[TestLog::s_uCountTab * SIZE_TAB] = '\0';
    return s_pcTab;
}
void TestLog::AddTab() { TestLog::s_uCountTab++; }
void TestLog::RemoveTab() { TestLog::s_uCountTab--; }
