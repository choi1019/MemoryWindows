//////////////////////////////////////////////////////////////
// Created 2019-01-21 by Sungwoon Choi
// All Rights Reserved
//////////////////////////////////////////////////////////////

#ifndef UID
#define UID "UId"
#include "../TypeDef.h"
#define UID_ID GETCLASSID(_eUId_)

#include "../../02Utility/String/String.h"

class UId {
private:
	String componentName;
	unsigned componentId;
	unsigned taskId;

public:
	UId() : taskId(UNDEFINED), componentId(UNDEFINED) {}
	UId(String& name, const UId &uId): componentName(name), componentId(uId.componentId), taskId(uId.taskId) {}
	UId(String& name, int componetId): componentName(name), componentId(componetId), taskId(UNDEFINED) {}
	UId(const String& name, int componetId, int taskId): componentName(name), componentId(componetId), taskId(taskId) {}

	inline const int getTaskId() { return this->taskId; }
	inline void setTaskId(int taskId) { this->taskId = taskId; }
	inline const int getComponentId() { return this->componentId; }
	inline String getComponentName() { return this->componentName; }
	inline UId& operator=(const UId& uId) {
		this->componentName = uId.componentName;
		this->componentId = uId.componentId;
		this->taskId = uId.taskId;
		return *this;
	}
};

static UId Make_UId(const String& componentName, const unsigned ComponentId, const unsigned taskId) {
	UId uId(componentName, ComponentId, taskId);
	return uId;
}
#endif
