#pragma once
#include "../TypeDef.h"
#define SOURCE "Source"  // class string name
#define SOURCE_ID GET_MODULE_ID(EDomain::eSource)  // class int id

#include "../../../../Core_Lib/Source/03Core/Component/Component.h"
#include "../../../../Technical_Lib/Source/05Technical/Timer/Timer.h"
#include "../../../../Core_Lib/Source/04Aspect/BasicIO/Display.h"

class Source : public Component {
public:
	enum class EEventType {
		eBegin = SOURCE_ID,
		eStart, eStartReply,
		eEnd
	};

	enum class ETargetGroups {
		eBegin = SOURCE_ID,
		eGroup1,
		eEnd
	};
protected:
	virtual void sense() = 0;

private:
	virtual void start(Event* pEvent) {
		PRINTLN("Source::start");
		this->sense();
		this->SendTargetEvents((unsigned)ETargetGroups::eGroup1, (unsigned)EEventType::eStart);
	}
	virtual void startReply(Event* pEvent) {
		PRINTLN("Source::startReply");
	}

public:
	Source(ClassInfo classInfo = ClassInfo(SOURCE_ID, SOURCE)): Component(classInfo) {
	}
	virtual ~Source() {}

	virtual void initialize() {
		// no timer
//		this->SendTargetEvents((unsigned)ETargetGroups::eGroup1, (unsigned)EEventType::eStart);
	}
	virtual void finalize() {}

	void processAEvent(Event* pEvent) {
		switch (pEvent->getType()) {
		case (unsigned)Timer::EEventType::eTimesUp:
			this->start(pEvent);
			break;
		case (unsigned)EEventType::eStartReply:
			this->startReply(pEvent);
			break;
		default:
			Component::processAEvent(pEvent);
		}
	}
};
