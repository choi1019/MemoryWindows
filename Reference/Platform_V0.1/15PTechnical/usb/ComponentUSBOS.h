/*
 * ComponentUSB.h
 *
 *  Created on: 2017. 11. 3.
 *      Author: comcsw
 */

#ifndef COMPONENTUSBOS_H_
#define COMPONENTUSBOS_H_
#include "../typedef.h"

#include "../../technical/device/Device.h"

class ComponentUSBOS: public Device {
private:
	void open(Msg *pMsg);
	void close(Msg *pMsg);
	void read(Msg *pMsg);
	void write(Msg *pMsg);
	void writeCompleted(Msg *pMsg);
public:
	ComponentUSBOS(String componentName, EDeviceId eDeiveceId);
	virtual ~ComponentUSBOS();
};

#endif /* COMPONENTUSBOS_H_ */
