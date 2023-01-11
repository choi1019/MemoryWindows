#pragma once

#include "../TypeDef.h"
#define PDISPLAY "PDisplay"  // class string name
#define PDISPLAY_ID GET_MODULE_ID(EPAspect::ePDisplay)  // class int id

#include "../../../../Core_Lib/Source/04Aspect/BasicIO/Display.h"
#include <stdio.h>

class PDisplay : public Display {
public:
	PDisplay(ClassInfo classInfo = ClassInfo(PDISPLAY_ID, PDISPLAY)) : Display(classInfo) {}
	virtual ~PDisplay() {}

	virtual void initialize() {}
	virtual void finalize() {}

	virtual void print(char* pContents) {
		printf("%s", pContents);
		fflush(stdout);
	}
	virtual void println(char* pContents) {
		printf("%s\n", pContents);
	}
};