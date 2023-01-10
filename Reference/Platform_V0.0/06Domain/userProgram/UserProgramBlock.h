/*
* UserProgramBlock.h
*
*  Created on: 2017. 10. 16.
*      Author: choi.sungwoon
*/

#ifndef USERPROGRAMBLOCK_
#define USERPROGRAMBLOCK_ "USERPROGRAMBLOCK_"

#include "../Typedef.h"
#include "../../technical/timer/Timer.h"

class UserProgramBlock {
private:
	void *pStartAddress;
	int size;
	int kind;
	bool bRun;
	int version;
public:
	UserProgramBlock() {}
	~UserProgramBlock() {}
	void initialize() {}
	void finalize() {}

	void start() {}
};
#endif /*USERPROGRAMBLOCK_*/
