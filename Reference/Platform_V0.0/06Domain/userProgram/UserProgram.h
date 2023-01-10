/*
* UserProgram.h
*
*  Created on: 2017. 10. 16.
*      Author: choi.sungwoon
*/

#ifndef USERPROGRAM_
#define USERPROGRAM_ "USERPROGRAM_"

#include "../Typedef.h"
#include "../../core/component/Component.h"
#include "UserProgramBlock.h"

class UserProgram: public Component {
private:
	void *pStartAddress;
	int size;
	int kind;

	Map<int, UserProgramBlock *> userProgramBlockMap;
public:
	UserProgram(String name): Component(name) {}
	~UserProgram() {}
	void initialize() {}
	void finalize() {}
};
#endif /*USERPROGRAM_*/