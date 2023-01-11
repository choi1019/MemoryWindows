#pragma once
#include "../TypeDef.h"
#define PMAIN "PMain"  // class string name
#define PMAIN_ID GET_MODULE_ID(EPApplication::ePMain)  // class int id

#include "../../../../Core_Lib/Source/09Application/Main/Main.h"
#include "../../../../POSAL_Lib/Source/19PApplication/PLifecycleManager/PLifecycleManager.h"
#include "../../../../POSAL_Lib/Source/13PCore/PEventQueue/PEventQueue.h"

class PMain : public Main
{
protected:
	// as a Component
	virtual void initialize() {}
	virtual void finalize() {}

	void createATask();

public:
	PMain(ClassInfo classInfo = ClassInfo(PMAIN_ID, PMAIN)): Main(new PEventQueue(), classInfo) {}
	virtual ~PMain() {}
};

