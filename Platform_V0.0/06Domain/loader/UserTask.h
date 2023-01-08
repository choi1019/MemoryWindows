/*
* UserTask.h
*
*  Created on: 2017. 10. 16.
*      Author: choi.sungwoon
*/

#ifndef USERTASK_
#define USERTASK_ "USERTASK_"

#include "../Typedef.h"
#include "../../osal/task/TaskCoreThreadOS.h"

/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

class UserTask : public TaskCoreThreadOS {

//////////////////////////////////
// event types
//////////////////////////////////
public:
	enum EEventType {
		eInitialize, eInitializeReply,
		eStart = __USERTASK, eStartReply,
		ePause, ePauseReply,
		eStop, eStopReply,
		eReset, eResetReply,
		eShutdown, eShutdownReply
	};

private:
	//////////////////////////////////
	// attributes
	//////////////////////////////////
	enum EState {  eCreated, eReady, ePaused, eRunning, eSuspended };
	EState eState;
};

/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

#endif /* USERTASK_ */
