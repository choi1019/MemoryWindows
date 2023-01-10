
#include "Device.h"

Device::Device(String componentName, EDeviceId deviceId)
	: Component(componentName), 
	deviceId(deviceId) 
{
}

Device::~Device()
{
}

// for interrupt processing
void Device::sendACallbackMsg(Msg* pMsg) {
//	this->getPTaskCore()->getPSystemMsgQueue()->send(pMsg);
}

//	initizlieDevice();
void Device::initialize() {
}

void Device::addMsgs() {
}

void Device::processMsg(Msg *pMsg) {
	switch (pMsg->getType()) {
	case eRead:
		this->read(pMsg);
		break;
	case eWrite:
		this->write(pMsg);
		break;
	case eWriteCompleted:
		this->writeCompleted(pMsg);
		break;
	default:
		break;
	}
}
