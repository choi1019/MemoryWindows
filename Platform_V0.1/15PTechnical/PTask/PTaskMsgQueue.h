//////////////////////////////////////////////////////////////
// Created 2019-01-21 by Sungwoon Choi
// All Rights Reserved
//////////////////////////////////////////////////////////////

#ifndef PTASKMSGQUEUE
#define PTASKMSGQUEUE "PTaskMsgQueue"
#include "../typedef.h"
#define PTASKMSGQUEUE_ID GETCLASSID(_ePTaskMsgQueue_)

#include "../../03Core/Task/TaskMsgQueue.h"
#include "../../02Utility/Queue/Queue.h"

class PTaskMsgQueue : public TaskMsgQueue {
private:
	_Queue<Message *, MAXTASKMSGQUEUELENGTH> msgQueue;

	SRWLOCK SRWLock;
	HANDLE *pEventHandle;

public:
	PTaskMsgQueue(HANDLE *pEventHandle);
	~PTaskMsgQueue();

	void initialize();
	void finalize();

	inline int add(Message *pMsg);
	inline Message* get();
	inline unsigned getLength();
};

#endif /* SYSTEMMSGQUEUEOS_ */
