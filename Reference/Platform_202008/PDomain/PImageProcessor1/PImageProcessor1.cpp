#include "PImageProcessor1.h"

PImageProcessor1::PImageProcessor1(ClassInfo classInfo)
	: ImageProcessor1(classInfo)
{
	this->addPart(&pImageProcessor1Part);
}

PImageProcessor1::~PImageProcessor1() {
}

void PImageProcessor1::start(Event* pEvent) {
//	cout << _methodName() << _endl;
	pImageProcessor1Part.algorithm1((unsigned)EReceivers::eImageProcess2, (unsigned)ImageProcessor2::EEventType::eStart);
}

void PImageProcessor1::startReply(Event* pEvent) {
//	cout << _methodName() << _endl;
}