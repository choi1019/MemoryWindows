//////////////////////////////////////////////////////////////
// Created 2019-01-21 by Sungwoon Choi
// All Rights Reserved
//////////////////////////////////////////////////////////////

#ifndef RECEIVER
#define RECEIVER "Receiver"
#include "../TypeDef.h"
#define RECEIVER_ID GETCLASSID(_eReceiver_)
#include "../../03Core/Component/Component.h"
#include "../../04Aspect/Exception/ExceptionManager.h"

class Receiver: public Component {
public:
	// event types
	enum EMessageType { 
		eMessageTypeStart = RECEIVER_ID,
		eFunction, eFunctionReply,
		eMessageTypeEnd
	};

	// reply structure
	class SFunctionReply {
	public:
		int result;
	};
	// send parameter structure
	class SFunctionParam: public ValueObject {
	public:
		int param1;
		int param2;
		SFunctionReply reply;

		SFunctionParam(): param1(0), param2(0) {
			reply.result = 0;
		}

		static MemoryManager *pMemoryManager;
	};

private:
	void function(Message *pMsg) {
		// get arguments
		SFunctionParam *pSFunctionParam = (SFunctionParam *)pMsg->getPArg();
		int p1 = pSFunctionParam->param1;
		int p2 = pSFunctionParam->param2;

		// compute result
		pSFunctionParam->reply.result = p1 + p2;
	}

	void processAFunction(Message *pMsg) {
		// process msg
		this->function(pMsg);
		// send reply
		this->addAReplyMsg(pMsg);
	}

public:
	Receiver(String name) : Component(name, Make_ClassInfo(RECEIVER_ID, RECEIVER)) {}
	virtual ~Receiver() {}

	void initialize() { Component::initialize(); }
	void finalize() { Component::finalize(); }

	void processAMsg(Message *pMsg) {
		switch (pMsg->getType()) {
		case eFunction:
			this->processAFunction(pMsg);
			break;
		default:
			Component::processAMsg(pMsg);
			break;
		}
	}
};


#endif /* RECEIVER_ */
