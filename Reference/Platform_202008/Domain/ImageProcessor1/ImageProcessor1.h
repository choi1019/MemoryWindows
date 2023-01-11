#pragma once
#include "../TypeDef.h"
#define IMAGEPROCESSOR1 "ImageProcessor1"  // class string name
#define IMAGEPROCESSOR1_ID GET_MODULE_ID(EDomain::eImageProcessor1)  // class int id

#include "../../../../Core_Lib/Source/03Core/Component/Component.h"
#include "../../../../Domain_Lib/Source/07Domain/ImageProcessor2/ImageProcessor2.h"
#include "../../../../Domain_Lib/Source/07Domain/Source/Source.h"
#include "ImageProcessor1Part.h"

class ImageProcessor1 : public Component {
public:
	enum class EEventType {
		eBegin = IMAGEPROCESSOR1_ID,
		eStart, eStartReply,
		eEnd
	};

	enum class EReceivers {
		eBegin = IMAGEPROCESSOR1_ID,
		eImageProcess2,
		eEnd
	};

	ImageProcessor1(ClassInfo classInfo = ClassInfo(IMAGEPROCESSOR1_ID, IMAGEPROCESSOR1));
	virtual ~ImageProcessor1();

	virtual void initialize() {}
	virtual void finalize() {}

	void processAEvent(Event* pEvent) {
		switch (pEvent->getType()) {
		case (unsigned)Source::EEventType::eStart:
			this->start(pEvent);
			this->SendAReplyEvent(pEvent);
			break;
		case (unsigned)ImageProcessor2::EEventType::eStartReply:
			this->startReply(pEvent);
			break;
		default:
			Component::processAEvent(pEvent);
		}
	}

protected:
	virtual void start(Event* pEvent) = 0;
	virtual void startReply(Event* pEvent) = 0;
};
