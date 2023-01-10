#pragma once

#include "../typedef.h"
#define _PSocketServerWorker_Id _GET_CLASS_UID(_ELayer_PTechnical::_ePSocketServerWorker)
#define _PSocketServerWorker_Name "PSocket"

#define DEFAULT_BUFLEN 512
#include "../../5PPlatform/PComponent/PComponentPart.h"
#include "../../6PTechnical/PProtocol/RSTP.h"

class PSocketServerWorker : public PComponentPart
{
public:
    enum class EException {
        eBegin,
        eShutdownFail,
        eWSAStartupFail,
        eGetAddrInfoFail,
        eSocketFail,
        eBindFail,
        eListenFail,
        eAcceptFail,
        eReceiveFail,
        eSendFail,
        eEnd
    };

	enum EState {
		eCreated,
		eBinded,
		eListening,
		eAccepted,
		eReceiving,
		eEnd
	};
private:
    RSTP* pRSTP;
    unsigned ClientSocket;

public:
    PSocketServerWorker(unsigned ClientSocket, int nClassId = _PSocketServerWorker_Id, const char* pcClassName = _PSocketServerWorker_Name);
    virtual ~PSocketServerWorker();
	virtual void Run();
    virtual void Start();

protected:
    virtual void Initialize();
	virtual void Finalize();

    virtual void Stop();
    virtual void Pause();
};