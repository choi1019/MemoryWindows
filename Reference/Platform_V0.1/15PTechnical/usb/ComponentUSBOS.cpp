/*
* ComponentUSB.cpp
*
*  Created on: 2017. 11. 3.
*      Author: comcsw
*/

#include "ComponentUSBOS.h"

ComponentUSBOS::ComponentUSBOS(String componentName, EDeviceId eDeiveceId)
	: Device(componentName, eDeiveceId) {
}

ComponentUSBOS::~ComponentUSBOS() {
}

void ComponentUSBOS::open(Msg *pMsg) {
}

void ComponentUSBOS::close(Msg *pMsg) {
}

void ComponentUSBOS::read(Msg *pMsg) {
}
void ComponentUSBOS::write(Msg *pMsg) {
}
void ComponentUSBOS::writeCompleted(Msg *pMsg) {
}
