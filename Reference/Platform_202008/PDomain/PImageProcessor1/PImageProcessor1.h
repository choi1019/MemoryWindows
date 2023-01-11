#pragma once
#include "../TypeDef.h"
#define PIMAGEPROCESSOR1 "PImageProcessor1"  // class string name
#define PIMAGEPROCESSOR1_ID GET_MODULE_ID(EPDomain::ePImageProcessor1)  // class int id

#include "../../../../Domain_Lib/Source/07Domain/ImageProcessor1/ImageProcessor1.h"
#include "../../../../Domain_Lib/Source/07Domain/ImageProcessor2/ImageProcessor2.h"
#include "PImageProcessor1Part.h"

class PImageProcessor1 : public ImageProcessor1 {
public:
	PImageProcessor1(ClassInfo classInfo = ClassInfo(PIMAGEPROCESSOR1_ID, PIMAGEPROCESSOR1));
	virtual ~PImageProcessor1();

protected:
	virtual void start(Event* pEvent);
	virtual void startReply(Event* pEvent);

private:
	PImageProcessor1Part pImageProcessor1Part;
};