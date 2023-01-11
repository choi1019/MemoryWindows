#include "PScheduler.h"

#include "../../../../FreeRTOS_Lib/Source/include/FreeRTOS.h"
#include "../../../../FreeRTOS_Lib/Source/include/task.h"

static void _TaskFunction(void* pParam) {
	Scheduler* pScheduler = (Scheduler*)pParam;
	pScheduler->run();
}

void PScheduler::createATask() {
	/* Create one of the two tasks. */
	xTaskCreate(
		_TaskFunction,	/* Pointer to the function that implements the task. */
		PSCHEDULER,		/* Text name for the task.  This is to facilitate debugging only. */
		1000,			/* Stack depth - most small microcontrollers will use much less stack than this. */
		this,			/* We are not using the task parameter. */
		1,				/* This task will run at priority 1. */
		NULL);			/* We are not using the task handle. */

}

void PScheduler::startATask() {}
