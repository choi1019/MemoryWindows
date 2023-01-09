#include "PTestMain.h"

int main() {
	try {
		PTestMain* pPTestMain = new PTestMain();
		pPTestMain->Initialize();
		pPTestMain->Run();
		pPTestMain->Finalize();
		delete pPTestMain;
	}
	catch (TestException& exception) {
		exception.Println();
	}

}