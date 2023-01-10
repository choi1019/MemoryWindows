/*
 * Device.h
 *
 *  Created on: 2017. 11. 3.
 *      Author: comcsw
 */

#ifndef DEVICE_
#define DEVICE_ "DEVICE_"
#define DEVICE_ID GETCLASSID(_eDevice_)

#include "../Typedef.h"
#include "../../03Core/Component/Component.h"
#include "../../03Core/Component/Message.h"


class Device : public Component {
public: 	
	enum EEventType { 
		eCallBack = DEVICE_ID,
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
	virtual void open(Message*pMsg) = 0;
	virtual void close(Message*pMsg) = 0;
	virtual void read(Message*pMsg) = 0;
	virtual void write(Message*pMsg) = 0;
	virtual void writeCompleted(Message*pMsg) = 0;

	// for interrupt processing
	void sendACallbackMsg(Message* pISRMsg);

public:
	Device(String componentName, EDeviceId deviceId);
	~Device();

	// getters & setters
	EDeviceId getDeviceId() { return this->deviceId; }

	// as a Component
	void initialize();
	void addMsgs();
	void processMsg(Message*pMsg);
};



#endif /* DEVICE_ */
