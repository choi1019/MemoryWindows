/*
* Loader.h
*
*  Created on: 2017. 10. 16.
*      Author: choi.sungwoon
*/

#ifndef LOADER_
#define LOADER_ "LOADER_"

#include "../Typedef.h"
#include "../../02Utility/Map/Map.h"
#include "../../03Core/Component/Message.h"
#include "../../03Core/Component/Component.h"
#include "../../03Core/Component/UId.h"

#include "LoaderData.h"
#include "../../05Technical/Timer/Timer.h"
#include "../../06Domain/loader/Executor.h"


class Seriaizable {
public:
	virtual void* read() {}
	virtual void write(void *) {}
};
class LabelTable : public Seriaizable {
};
class ForNextTable : public Seriaizable {
};
class CommandBaseLib : public Seriaizable {
};
class MotionFunctionLib : public Seriaizable {
};
class OSServiceFunctionTable : public Seriaizable {
};
class UnhandledExceptionTable : public Seriaizable {
};


class Constants : public Seriaizable {
};
class RetainData : public Seriaizable {
};
class DataSegment : public Seriaizable {
};
class DeviceIO : public Seriaizable {
};
class Stack : public Seriaizable {
};

class UserProgramBlock : public Seriaizable {
private:
	int id;
	void* startAddress;
	int size;
	int kind;
	bool bExecutable;
	int version;
public:

};

class UserProgram {
private:

	Map<int, UserProgramBlock> userProgramBlockMap;

	////////////////////////////////////////////
	// code segment related
	////////////////////////////////////////////
	LabelTable labelTable;
	ForNextTable forNextTable;
	CommandBaseLib commandBaseLib;
	MotionFunctionLib motionFunctionLib;
	OSServiceFunctionTable oSServiceFunctionTable;
	UnhandledExceptionTable unhandledExceptionTable;

	////////////////////////////////////////////
	// data segment related
	////////////////////////////////////////////
	Constants Constants;
	RetainData RetainData;
	DataSegment DataSegment;
	DeviceIO DeviceIO;
	Stack Stack;

public:

};

class Loader : public Component {
private:
	LoaderData *pLoaderDataArray[eLoaderDataSize];

	// receiver address
	UId executorUId;

	// read data from input buffer
	ELoaderDataIndex readData();

	// sychronized reply
	bool bFunctionWait;
	// send a msg to the receiver
	void sendData();
	// process a reply msg from the receiver
	void sendDataReply(Message*pMsg);

public:
	Loader(String componentName): Component(componentName) {}
	virtual ~Loader() {}

	void initialize();
	void finalize();

	void associateAExecutor(UId componentUId);

	// as a Component
	void processAMsg(Message*pMsg);
};

#endif /* SENDER_ */
