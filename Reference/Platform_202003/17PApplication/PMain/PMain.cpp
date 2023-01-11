//////////////////////////////////////////////////////////////
// Created 2019-01-21 by Sungwoon Choi
// All Rights Reserved
//////////////////////////////////////////////////////////////
#include "../../15PTechnical/PMemoryManager/PMemory.h"
#include "../../17PApplication/PMain/PMainTask.h"
#include "../../07Application/LifecycleManager/LifecycleManager.h"
#include "../../04Aspect/Exception/ExceptionManager.h"

int main() {
	PMainTask mainTask("PMainTask");
	while (true) {
		mainTask.startCore();
		mainTask.finalize();
	}
}
