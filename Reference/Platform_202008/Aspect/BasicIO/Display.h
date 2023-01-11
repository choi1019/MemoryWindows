#pragma once
#include "../TypeDef.h"
#define DISPLAY "Display"  // class string name
#define DISPLAY_ID GET_MODULE_ID(EAspect::eDisplay)  // class int id

#include "../../../../Core_Lib/Source/03Core/Component/Component.h"

class Display : public Component {
public:
	enum class EEventType {
		eBegin = DISPLAY_ID,
		ePrint, ePrintReply,
		ePrintln, ePrintlnReply,
		eEnd
	};

	enum class EReceivers {
		eBegin = DISPLAY_ID,
		eEnd
	};

	Display(ClassInfo classInfo = ClassInfo(DISPLAY_ID, DISPLAY)): Component(classInfo) {}
	virtual ~Display() {}

	virtual void initialize() {}
	virtual void finalize() {}

	virtual void print(char* pContents) {}
	virtual void println(char* pContents) {}

	void processAEvent(Event* pEvent) {
		switch (pEvent->getType()) {
		case (unsigned)EEventType::ePrint:
			this->print(pEvent);
//			this->SendAReplyEvent(pEvent);
			break;
		case (unsigned)EEventType::ePrintln:
			this->println(pEvent);
			break;
		default:
			Component::processAEvent(pEvent);
		}
	}
private:
	virtual void print(Event *pEvent) {
		this->print((char *)pEvent->getPArg());
	}
	virtual void println(Event* pEvent) {
		this->println((char *)pEvent->getPArg());
	}
};