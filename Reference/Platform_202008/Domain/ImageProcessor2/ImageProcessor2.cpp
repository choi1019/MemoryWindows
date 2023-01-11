#include "ImageProcessor2.h"

// standard interface
ImageProcessor2::ImageProcessor2(ClassInfo classInfo) 
	: Component(classInfo) 
{
}

ImageProcessor2::~ImageProcessor2() 
{
}

void ImageProcessor2::initialize() 
{
}

void ImageProcessor2::finalize() 
{
}

// variability
void ImageProcessor2::algorithm2() {
	PRINTLN("ImageProcessor2::algorithm2");
}

// commonality
void ImageProcessor2::start(Event* pEvent) {
	this->algorithm2();
	this->SendAReplyEvent(pEvent);
}

void ImageProcessor2::processAEvent(Event* pEvent) {
	switch (pEvent->getType()) {
	case (unsigned)EEventType::eStart:
		this->start(pEvent);
		break;
	default:
		Component::processAEvent(pEvent);
	}
}

