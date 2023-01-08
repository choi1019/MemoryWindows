#pragma once

#ifndef PTESTCASE
#define PTESTCASE "PTESTCASE"
#include "../Typedef.h"
#define PTESTCASE_ID GETCLASSID(_ePTestCase_)

#include "../../09Test/TestCase/TestCase.h"
#include "../../15PTechnical/PMemoryManager/PMemory.h"
#include "../../02Utility/Vector/Vector.h"
#include "../../03Core/Component/Message.h"

class PTestCase : public TestCase
{
private:
	_Vector<Message*> *pMessages;

public:
	PTestCase() {
		this->pMessages = new _Vector<Message*>();
	}
	virtual ~PTestCase() {
		delete this->pMessages;
	}

	virtual void initialize() {
		for (unsigned i = 0; i < this->pMessages->getMaxLength(); i++) {
			Message* pMessage = new(Message::pMemoryManager) Message(1, *(new UId()), *(new UId()), 1, NULL, 1);
			this->pMessages->add(pMessage);
		}
	}
	virtual void finalize() {
		for (ValueObject* pValueObject: *(this->pMessages)) {
			pValueObject->destroy(Message::pMemoryManager);
		}
	}

	virtual void run() {
		for (unsigned i = 0; i < this->pMessages->getMaxLength() + 1; i++) {
			if (i < this->pMessages->getMaxLength()) {
				Message* pMessage = pMessages->get(i);
				printf("%d, %s, %d, \n", pMessage->getClassId(), pMessage->getClassName(), pMessage->getObjectId());
			}
			else {
				throw(exception("exception test")); 
			}
		}
	}
};

#endif
