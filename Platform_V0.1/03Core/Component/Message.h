//////////////////////////////////////////////////////////////
// Created 2019-01-21 by Sungwoon Choi
// All Rights Reserved
//////////////////////////////////////////////////////////////

#ifndef MESSAGE
#define MESSAGE "Message"
#include "../TypeDef.h"
#define MESSAGE_ID GETCLASSID(_eMessage_)

#include "../../01Global/Object/ValueObject.h"
#include "../../03Core/Component/UId.h"

#define NOREPLY -1

class Message : public ValueObject
{
private:
	static int sMessageId;

	int id;
	int type;
	UId targetUId;
	UId sourceUId;
	int iArg;
	ValueObject *pArg;
	int replyType;

public:
	Message(Message *pMessage): 
		id(sMessageId++), type(pMessage->getType()), 
		targetUId(pMessage->getTargetUId()), sourceUId(pMessage->getSourceUId()),
		iArg(pMessage->getIArg()), pArg(pMessage->getPArg()), replyType(pMessage->getReplyType()),
		ValueObject(Make_ClassInfo(MESSAGE_ID, MESSAGE)) {
	}
	Message(int type, const UId &sourceUId, const UId &targetUId, int iArg, ValueObject *pArg, int replyType):
		id(sMessageId++), type(type), 
		sourceUId(sourceUId), targetUId(targetUId), 
		iArg(iArg), pArg(pArg), replyType(replyType),
		ValueObject(Make_ClassInfo(MESSAGE_ID, MESSAGE)) {
	}
	virtual ~Message() {
	}

	inline int getId() { return this->id; }
	inline int getType() { return this->type; }

	inline const UId &getTargetUId() { return this->targetUId; }
	inline const int getTargetTaskId() { return this->targetUId.getTaskId(); }
	inline const int getTargetComponentId() { return this->targetUId.getComponentId(); }

	inline const UId &getSourceUId() { return this->sourceUId; }
	inline const int getSourceTaskId() { return this->sourceUId.getTaskId(); }
	inline const int getSourceComponentId() { return this->sourceUId.getComponentId(); }

	inline int getIArg() { return this->iArg; }
	inline ValueObject* getPArg() { return this->pArg; }
	inline int getReplyType() { return this->replyType; }
};

#endif