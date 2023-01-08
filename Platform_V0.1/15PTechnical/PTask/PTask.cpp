//////////////////////////////////////////////////////////////
// Created 2019-01-21 by Sungwoon Choi
// All Rights Reserved
//////////////////////////////////////////////////////////////

#include "PTask.h"
#include "../../05Technical/Timer/Timer.h"

 /////////////////////////////////////////////////////////
 // thread entry function
DWORD WINAPI _ThreadProc(LPVOID lpParam) {
	Task *pTask = (Task *)lpParam;
	pTask->processTaskMsgs();
	return 1;
}

void PTask::startATask() {
	this->hThread = CreateThread(
		NULL,			// default security
		0,				// default stack size
		_ThreadProc,	// name of the thread function
		this,			// thread parameters
		0,				// default startup flags
		&dwThreadID
	);
	if (this->hThread == NULL)
	{
		printf("%s::startATask() === Error \n", this->getComponentName().c_str());
	}
	printf("%s::startATask()\n", this->getComponentName().c_str());
}

void PTask::stopATask() {
	Task::stopATask();
	CloseHandle(this->hThread);
}
/////////////////////////////////////////////////////////

PTask::PTask(String name) : 
	hThread(NULL), 
	taskMsgQueue(&this->hEvents[eTaskMsgQueue]), componentMsgVector(&this->hEvents[eComponentMsgQueue]),
	Task(name, Make_ClassInfo(PTASK_ID, PTASK), &taskMsgQueue, &componentMsgVector)
{
}

// for MainTask
PTask::PTask(String name, ClassInfo classInfo) :
	hThread(NULL), taskMsgQueue(&this->hEvents[eTaskMsgQueue]), componentMsgVector(&this->hEvents[eComponentMsgQueue]),
	Task(name, classInfo, &taskMsgQueue, &componentMsgVector)
{
}

// for Timer
PTask::PTask(String name, ClassInfo classInfo, HANDLE *pHTaksMsgQueue, HANDLE *pHComponentMsgQueue) :
	hThread(NULL), taskMsgQueue(pHTaksMsgQueue), componentMsgVector(pHComponentMsgQueue),
	Task(name, classInfo, &taskMsgQueue, &componentMsgVector)
{
}

PTask::~PTask()
{
}

void PTask::initialize() {
	Task::initialize();
}

void PTask::finalize() {
	Task::finalize();
	
}

void PTask::processATaskMsg() {
	DWORD dwResult;
	dwResult = WaitForMultipleObjects(
		eESynchronizationObjectsEnd,	// number of objects in array
		this->hEvents,	// array of objects
		FALSE,			// wait for any object
		INFINITE		// wait forever
	);

	switch (dwResult) {
	case WAIT_OBJECT_0 + eComponentMsgQueue:
		// hHandles[1] was signaled
		this->collectMsgs();
		ResetEvent(this->hEvents[eComponentMsgQueue]);
		break;
	case WAIT_OBJECT_0 + eTaskMsgQueue:
		// hHandles[2] was signaled
		this->distributeAMsg();
		ResetEvent(this->hEvents[eTaskMsgQueue]);
		break;
	case WAIT_TIMEOUT:
		printf("Wait timed out.\n");
		break;
	default:
		// Return value is invalid.
		printf("Wait error: %d\n", GetLastError());
		ExitProcess(0);
	}
}

void PTask::processAMsg(Message *pMessage) {
	Task::processAMsg(pMessage);
}
