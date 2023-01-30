#include <22TestApplication/PTestMain/PTestMain.h>
#include <21TestPlatform/TestAspect/TestException.h>

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