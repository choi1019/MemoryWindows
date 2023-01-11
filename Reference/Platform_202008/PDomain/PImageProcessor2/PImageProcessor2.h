#pragma once
#include "../TypeDef.h"
#define PIMAGEPROCESSOR2 "PImageProcessor2"  // class string name
#define PIMAGEPROCESSOR2_ID GET_MODULE_ID(EPDomain::ePImageProcessor2)  // class int id

#include "../../../../Domain_Lib/Source/07Domain/ImageProcessor2/ImageProcessor2.h"

class PImageProcessor2 : public ImageProcessor2 {

protected:
	virtual void algorithm2() {
		ImageProcessor2::algorithm2();
	}
public:
	PImageProcessor2(ClassInfo classInfo = ClassInfo(PIMAGEPROCESSOR2_ID, PIMAGEPROCESSOR2))
	: ImageProcessor2(classInfo)
	{}
	virtual ~PImageProcessor2() {}
};