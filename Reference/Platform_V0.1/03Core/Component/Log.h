//////////////////////////////////////////////////////////////
// Created 2019-01-21 by Sungwoon Choi
// All Rights Reserved
//////////////////////////////////////////////////////////////

#ifndef LOG
#define LOG "Log"
#include "../TypeDef.h"
#define LOG_ID GETCLASSID(_eLog_)

#include "../../01Global/Object/ValueObject.h"
#include "UId.h"

#define MAXLOGSIZE 30
class Log: public ValueObject {
private:
	static unsigned sLogId;
public:

	enum EMessageType {
		mMessageTypeBegin = LOG_ID,
		mLog, mLogReply,
		mWarning, mWarningReply,
		mError, mErrorReply,
		mMessageTypeEnd
	};

private:
	unsigned id;
	unsigned type;
	UId uId;
	const char *methodName;
	unsigned lineNo;

public:
	static MemoryManager *pMemoryManager;

	Log(unsigned type, const UId uId, const char *methodName, const unsigned lineNo) :
		id(sLogId++), type(type), uId(uId), methodName(methodName), lineNo(lineNo) {}
	~Log() {}

	unsigned getType() { return this->type; }
	UId getUId() { return this->uId; }
	unsigned getTaskId() { return this->uId.getTaskId(); }
	unsigned getComponentId() { return this->uId.getComponentId(); }
	const char *getMethodName() { return this->methodName; }
	unsigned getLineNo() { return this->lineNo; }
};

#endif
