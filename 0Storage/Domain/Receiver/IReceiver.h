#pragma once

#include "../typedef.h"
#define _Receiver_Id _GET_CLASS_UID(_ELayer_Domain::_eReceiver)
#define _Receiver_Name "Receiver"

#include "../../../Platform/1Base/Object/BaseObject.h"
#include "../../../Platform/1Base/StdLib/String.h"

class IReceiver {
public:
	///////////////////////////////////////
	// Component Parts
	///////////////////////////////////////
	enum class EPart {
		eBegin = _Receiver_Id,
		eReceiverPart,
		eEnd
	};

	///////////////////////////////////////
	// Provuded Interface
	///////////////////////////////////////
	// event type
	enum class EEventType {
		eBegin = _Receiver_Id,
		eCompute,
		eEnd
	};

	//////////////////////////////////////
	// Process : Parameter & Result Type
	//////////////////////////////////////
	// paramenter
	class ParamCompute : public BaseObject {
	private:
		int m_nAtt1;
		String m_sAtt2;

	public:
		ParamCompute(int nAtt1, String sAtt2) {
			m_nAtt1 = nAtt1;
			m_sAtt2 = sAtt2;
		}
		~ParamCompute() {
		}
		int GetAtt1() { return this->m_nAtt1; }
		String GetAtt2() { return this->m_sAtt2; }
	};

	// result
	class Result : public BaseObject {
	private:
		int m_nAtt1;
		String m_sAtt2;
	public:
		Result(int nAtt1, String sAtt2) {
			m_nAtt1 = nAtt1;
			m_sAtt2 = sAtt2;
		}
		~Result() {
		}
		int GetAtt1() { return this->m_nAtt1; }
		String GetAtt2() { return this->m_sAtt2; }
	};
};