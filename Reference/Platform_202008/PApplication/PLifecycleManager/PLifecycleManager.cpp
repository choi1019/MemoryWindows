#include "PLifecycleManager.h"

#include "../../../../FreeRTOS_Lib/Source/include/FreeRTOS.h"
#include "../../../../FreeRTOS_Lib/Source/include/task.h"

void PLifecycleManager::startTasks() {
	vTaskStartScheduler();
}