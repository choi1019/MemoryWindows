/*
 * Device.h
 *
 *  Created on: 2017. 11. 3.
 *      Author: comcsw
 */

#ifndef DEVICE_
#define DEVICE_ "DEVICE_"

#include "../Typedef.h"
#include "../../core/component/Component.h"

class Device : public Component {
public: 	
	enum EEventType { 
		eCallBack = __DEVICE,
		eOpen, 
		eClose,
		eRead, 
		eWrite, eWriteCompleted 
	};
	enum EDeviceId { eFirst, eSecond, eCallbackSize };

private:
	EDeviceId deviceId;

protected:
	// device Interface
	virtual void open(Msg *pMsg) = 0;
	virtual void close(Msg *pMsg) = 0;
	virtual void read(Msg *pMsg) = 0;
	virtual void write(Msg *pMsg) = 0;
	virtual void writeCompleted(Msg *pMsg) = 0;

	// for interrupt processing
	void sendACallbackMsg(Msg* pISRMsg);

public:
	Device(String componentName, EDeviceId deviceId);
	~Device();

	// getters & setters
	EDeviceId getDeviceId() { return this->deviceId; }

	// as a Component
	void initialize();
	void addMsgs();
	void processMsg(Msg *pMsg);
};



#endif /* DEVICE_ */
