/*
* SourceComponent.cpp
*
*  Created on: 2017. 10. 16.
*      Author: choi.sungwoon
*/

#include "Executor.h"

void Executor::associateAUserTask(ComponentUId componentUId) {
	Component::associateAReceiver(componentUId);
	this->userTaskUId = componentUId;
}

void Executor::processProgramData(Msg *pMsg) {
	switch (pMsg->getIArg()) {
	case eBinaryIndex:
		break;
	case eLabelMappingTableIndex:
		break;
	case eTaskTableIndex:
		break;
	case eProgramBlockTableIndex:
		break;
	default:
		break;
	}
}

void Executor::processAMsg(Msg *pMsg) {
	switch (pMsg->getType()) {
	case Executor::eProgramData:
		printComponentMsg(this, "eProgramData", pMsg);
		processProgramData(pMsg);
		this->addAReplyMsg(pMsg);
		break;
	default:
		break;
	}
}