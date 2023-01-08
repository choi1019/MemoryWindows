//////////////////////////////////////////////////////////////
// Created 2019-01-21 by Sungwoon Choi
// All Rights Reserved
//////////////////////////////////////////////////////////////

#ifndef TASKMSGQUEUE
#define TASKMSGQUEUE "TaskMsgQueue"
#include "../typedef.h"
#define TASKMSGQUEUE_ID GETCLASSID(_eTaskMsgQueue_)

#include "../../03Core/component/Message.h"
#include "../../01Global/Object/Object.h"

class TaskMsgQueue: public Object {
public:
	TaskMsgQueue(): Object(Make_ClassInfo(TASKMSGQUEUE_ID, TASKMSGQUEUE)) {}
	virtual ~TaskMsgQueue() {}
	virtual void initialize() { Object::initialize();}
	virtual void finalize() { Object::finalize(); }

	virtual inline int add(Message *pMsg) = 0;
	virtual inline Message* get() = 0;
	virtual inline unsigned getLength() = 0;
};

#endif /* MSGQUEUE */
