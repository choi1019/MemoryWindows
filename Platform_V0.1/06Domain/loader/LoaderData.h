/*
* UserTask.h
*
*  Created on: 2017. 10. 16.
*      Author: choi.sungwoon
*/

#ifndef LOADERDATA_
#define LOADERDATA_ "LOADERDATA_"

#include "../../01Global/Object/Object.h"

enum ELoaderDataIndex {
	eBinaryIndex,
	eLabelMappingTableIndex,
	eTaskTableIndex,
	eProgramBlockTableIndex,
	eLoaderDataSize
};

class LoaderData: public Object {
public:
	virtual void generate() = 0;
};
class Binary : public LoaderData {
public:
	void generate() {
	}
};
class LabelMappingTable : public LoaderData {
public:
	void generate() {
	}
};
class TaskTable : public LoaderData {
public:
	void generate() {
	}
};
class ProgramBlockTable : public LoaderData {
public:
	void generate() {
	}
};

#endif /*LOADERDATA_*/