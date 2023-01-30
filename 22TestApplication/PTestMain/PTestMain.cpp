
#include <22TestApplication/PTestMain/PTestMain.h>

#include <21TestPlatform/TestAspect/TestException.h>
#include <22TestApplication/PTS1/PTS1.h>

PTestMain::PTestMain(unsigned nClassId, const char* pcClassName)
	: TestMain(nClassId, pcClassName)
{
}
PTestMain::~PTestMain() {
}

void PTestMain::InitializeMain() {
	TestMain::InitializeMain();
}
void PTestMain::RunMain() {
	TestMain::RunMain();
}
void PTestMain::FinalizeMain() {
	TestMain::FinalizeMain();
}

void PTestMain::Initialize() {
	// <int SIZE_SYSTEM_MEMORY, int SIZE_USER_MEMORY, int SIZE_PAGE, int SIZE_SLOT_UNIT>
	this->add(new("PTS1-1") PTS1<4096, 6144, 1024, 32>());
//		this->add(new("PTS1-2") PTS1<4096, 6144, 256, 8>());
//		this->add(new("PTS1-3") PTS1<4096, 8192, 2048, 128>());
//		this->add(new("PTS2-1") PTS2());
}

void PTestMain::Finalize() {
}

//#include <22TestApplication/PTestMain/PTestMain.h>
//#include <21TestPlatform/TestAspect/TestException.h>

int main() {
	try {
		PTestMain *pPTestMain = new("PTestMain") PTestMain();
		pPTestMain->InitializeMain();
		pPTestMain->RunMain();
		pPTestMain->FinalizeMain();
		delete pPTestMain;
	}
	catch (TestException& exception) {
		exception.Println();
	}
	return 0;
}