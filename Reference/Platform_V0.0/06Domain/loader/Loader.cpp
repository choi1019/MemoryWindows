/*
* SourceComponent.cpp
*
*  Created on: 2017. 10. 16.
*      Author: choi.sungwoon
*/

#include "Loader.h"

void Loader::initialize() {
	this->pLoaderDataArray[eBinaryIndex]			= new Binary();
	this->pLoaderDataArray[eLabelMappingTableIndex] = new LabelMappingTable();
	this->pLoaderDataArray[eTaskTableIndex]			= new TaskTable();
	this->pLoaderDataArray[eProgramBlockTableIndex] = new ProgramBlockTable();
}
void Loader::finalize() {
	for (int i = 0; i < eLoaderDataSize; i++) {
		delete(this->pLoaderDataArray[i]);
	}
}

void Loader::associateAExecutor(ComponentUId componentUId) {
	Component::associateAReceiver(componentUId);
	this->executorUId = componentUId;
}

ELoaderDataIndex Loader::readData() {
	return eBinaryIndex;
}

// send a msg to the receiver
void Loader::sendData() {
	ELoaderDataIndex eLoaderDataIndex = readData();
	this->pLoaderDataArray[eLoaderDataIndex]->generate();
	this->addASenderMsg(Executor::eProgramData, executorUId, eLoaderDataIndex, (Object*)this->pLoaderDataArray[eLoaderDataIndex]);
}

// process a reply msg from the receiver
void Loader::sendDataReply(Msg *pMsg) {
}

void Loader::processAMsg(Msg *pMsg) {
	switch (pMsg->getType()) {
	case Timer::eTimesUp:
		printComponentMsg(this, "eTimesUp", pMsg);
		this->sendData();
		break;
	case Executor::eProgramDataReply:
		printComponentMsg(this, "eProgramDataReply", pMsg);
		this->sendDataReply(pMsg);
		break;
	default:
		break;
	}
}