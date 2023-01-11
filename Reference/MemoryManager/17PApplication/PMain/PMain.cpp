//////////////////////////////////////////////////////////////
// Created 2019-01-21 by Sungwoon Choi
// All Rights Reserved
//////////////////////////////////////////////////////////////
#include "../../15PTechnical/PMemoryManager/PMemory.h"
#include "../../17PApplication/PMain/PMainTask.h"

int main() {
	PMainTask mainTask;
	while (true) {
		mainTask.startCore();
		mainTask.finalize();
	}
}
