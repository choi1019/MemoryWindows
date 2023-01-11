/*
* Executor.h
*
*  Created on: 2017. 10. 16.
*      Author: choi.sungwoon
*/

#ifndef EXECUTOR_
#define EXECUTOR_ "EXECUTOR_"

#include "../Typedef.h"
#include "../../core/component/Component.h"

#include "LoaderData.h"
#include "../../domain/loader/UserTask.h"

class Executor : public Component {
public:
	enum EEventType {
		eProgramData = __EXECUTOR, eProgramDataReply,
	};

private:
	// receiver address
	ComponentUId userTaskUId;

	Map<int, TaskCore*> taskCoreMap;
	Map<int, Component *> userProgramMap;

public:
	Executor(String componentName): Component(componentName) {}
	virtual ~Executor() {}

	void associateAUserTask(ComponentUId componentUId);

	void processProgramData(Msg *pMsg);
	void processAMsg(Msg *pMsg);
};

#endif /* EXECUTOR_ */