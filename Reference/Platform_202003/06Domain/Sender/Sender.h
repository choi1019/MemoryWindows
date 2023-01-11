//////////////////////////////////////////////////////////////
// Created 2019-01-21 by Sungwoon Choi
// All Rights Reserved
//////////////////////////////////////////////////////////////

#ifndef SENDER
#define SENDER "Sender"
#include "../TypeDef.h"
#define SENDER_ID GETCLASSID(_eSender_)
#include "../../03Core/Component/Component.h"
#include "../../04Aspect/Exception/ExceptionManager.h"

#include "../../06domain/receiver/Receiver.h"
#include "../../05Technical/Timer/Timer.h"


class Sender: public Component {
private:
	// receiver address
	UId receiverUId;
	inline void processATimesUp(Message *pMsg) {
		this->sendAFunctionMsg();
	}

	// send a msg to the receiver
	inline void sendAFunctionMsg() {
		//////////////////////////////////////////////////
		Receiver::SFunctionParam *pSFunctionParam = new(Receiver::SFunctionParam::pMemoryManager) Receiver::SFunctionParam();
		if (pSFunctionParam != NULL) {
			pSFunctionParam->param1 = 1;
			pSFunctionParam->param2 = 2;

			// add the msg to the queue
			this->addAReceiverMsg(Receiver::eFunction, receiverUId, 0, pSFunctionParam, Receiver::eFunctionReply);
		}
	}


	// process a reply msg from the receiver
	inline void processAFunctionReply(Message *pMsg) {
		int iArg = pMsg->getId();
		const ValueObject *pArg = pMsg->getPArg();
		Receiver::SFunctionParam *pSFunctionParam = (Receiver::SFunctionParam *)pArg;
		int result = pSFunctionParam->reply.result;

		///////////////////////////////////////////////////////////////////////
		// delete memory
		pSFunctionParam->~SFunctionParam();
		Receiver::SFunctionParam::pMemoryManager->safeFree(pSFunctionParam);
		///////////////////////////////////////////////////////////////////////
		// send a exception message
		Log *pLog = new(Log::pMemoryManager) Log(0, this->getUId(), __FUNCTION__, __LINE__);
		this->addAReceiverMsg(Log::mLog, this->getExceptionManager()->getUId(), 0, pLog);
		///////////////////////////////////////////////////////////////////////
//		printf("processAFunctionReply - %d\n", result);
	}

public:
	Sender(String name) : Component(name, Make_ClassInfo(SENDER_ID, SENDER)) {}
	virtual ~Sender() {}
	inline void initialize() {	Component::initialize(); }
	inline void finalize() { Component::finalize(); }

	inline void associateAReceiver(Component *pComponent) {
		Component::associateAReceiver(pComponent);
		Receiver *pReceiver = dynamic_cast<Receiver *>(pComponent);
		if (pReceiver) {
			this->receiverUId = pReceiver->getUId();
		}
	}

	void processAMsg(Message *pMsg) {
		switch (pMsg->getType()) {
		case Timer::eTimesUp:
			this->processATimesUp(pMsg);
			break;
		case Receiver::eFunctionReply:
			this->processAFunctionReply(pMsg);
			break;
		default:
			Component::processAMsg(pMsg);
			break;
		}
	}
};

#endif /* SENDER_ */
