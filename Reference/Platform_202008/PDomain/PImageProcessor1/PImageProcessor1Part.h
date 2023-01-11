#pragma once
#include "../TypeDef.h"
#define PIMAGEPROCESSOR1PART "PImageProcessor1Part"  // class string name
#define PIMAGEPROCESSOR1PART_ID GET_MODULE_ID(EPDomain::ePImageProcessor1Part)  // class int id

#include "../../../../Domain_Lib/Source/07Domain/ImageProcessor1/ImageProcessor1Part.h"

class PImageProcessor1Part : public ImageProcessor1Part {
public:
	PImageProcessor1Part(ClassInfo classInfo = ClassInfo(PIMAGEPROCESSOR1PART_ID, PIMAGEPROCESSOR1PART))
		: ImageProcessor1Part(classInfo)
	{}
	virtual ~PImageProcessor1Part() {}

	virtual void algorithm1(unsigned targetComponentName, unsigned eventType) {
		ImageProcessor1Part::algorithm1(targetComponentName, eventType);
//		cout << "PImageProcessor1Part::algorithm1()" << _endl;
	}
};