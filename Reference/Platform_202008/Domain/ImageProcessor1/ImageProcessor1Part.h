#pragma once
#include "../TypeDef.h"
#define IMAGEPROCESSOR1PART "ImageProcessor1Part"  // class string name
#define IMAGEPROCESSOR1PART_ID GET_MODULE_ID(EDomain::eImageProcessor1Part)  // class int id

#include "../../../../Core_Lib/Source/03Core/Component/ComponentPart.h"

class ImageProcessor1Part : public ComponentPart {
public:
	ImageProcessor1Part(ClassInfo classInfo = ClassInfo(IMAGEPROCESSOR1PART_ID, IMAGEPROCESSOR1PART)):
		ComponentPart(classInfo)
	{
	}
	virtual ~ImageProcessor1Part() {}

	virtual void algorithm1(unsigned targetComponentName, unsigned eventType) {
		PRINTLN("ImageProcessor1Part::algorithm1");
		this->SendAEvent(targetComponentName, eventType);
	};
};
