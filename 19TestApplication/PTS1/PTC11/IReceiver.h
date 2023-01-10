#pragma once

#include "../../typedef.h"
#define _PTC41_RECEIVER_NAME_ "Receiver"
#define _PTC41_RECEIVER_ID_ _GET_CLASS_UID_(_EPTestApplication::_ePTC41_Receiver)

#include "../../../13Component/Component/Component.h"
#include "../../../11Object/StdLib/String.h"

class IReceiver {
public:
	///////////////////////////////////////
	// Component Parts
	///////////////////////////////////////
	enum class EPart {
		eBegin = _PTC41_RECEIVER_ID_,
		eReceiverPart,
		eEnd
	};

	///////////////////////////////////////
	// Provuded Interface
	///////////////////////////////////////
	// event type
	enum class EEventType {
		eBegin = _PTC41_RECEIVER_ID_,
		eTest, eTestReply,
		eEnd
	};

	// Test event paramenter
	class ParamTest : public BaseObject {
	private:
		int m_iAtt1;
		String m_sAtt2;

	public:
		ParamTest() {
			m_iAtt1 = 100;
			m_sAtt2 = "Test";
		}
		~ParamTest() {
		}
		int GetAtt1() { return this->m_iAtt1; }
		String GetAtt2() { return this->m_sAtt2; }
	};

	// Test event reply
	class ResultTest : public BaseObject {
	private:
		int m_iResult;
	public:
		ResultTest() {
			m_iResult = 200;
		}
		~ResultTest() {
		}
		int GetResult() { return this->m_iResult; }
	};
};