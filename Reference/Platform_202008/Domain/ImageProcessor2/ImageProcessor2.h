#pragma once
#include "../TypeDef.h"
#define IMAGEPROCESSOR2 "ImageProcessor2"  // class string name
#define IMAGEPROCESSOR2_ID GET_MODULE_ID(EDomain::eImageProcessor2)  // class int id

#include "../../../../Core_Lib/Source/03Core/Component/Component.h"
#include "../../../../Domain_Lib/Source/07Domain/Source/Source.h"

class ImageProcessor2 : public Component {
public:
	// interface
	enum class EEventType {
		eBegin = IMAGEPROCESSOR2_ID,
		eStart, eStartReply,
		eeEnd
	};

	enum class EReceivers {
		eBegin = IMAGEPROCESSOR2_ID,
		eEnd
	};
protected:
	// variability
	virtual void algorithm2();

private:
	// commonality
	void start(Event* pEvent);

public:
	// standard interface
	ImageProcessor2(ClassInfo classInfo = ClassInfo(IMAGEPROCESSOR2_ID, IMAGEPROCESSOR2));
	virtual ~ImageProcessor2();
	virtual void initialize();
	virtual void finalize();

	void processAEvent(Event* pEvent);
};
