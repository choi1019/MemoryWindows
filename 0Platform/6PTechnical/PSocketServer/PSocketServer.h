#pragma once

#include "../typedef.h"
#define _PSocketServer_Id _GET_CLASS_UID(_ELayer_PTechnical::_ePSocketServer)
#define _PSocketServer_Name "PSocket"

#include "../../3Technical/SocketServer/SocketServer.h"
#include "../../5PPlatform/PComponent/PComponent.h"

#define DEFAULT_PORT "27015"

class PSocketServer : public PComponent, public SocketServer
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
public:
    PSocketServer(int nClassId = _PSocketServer_Id, const char* pcClassName = _PSocketServer_Name);
    virtual ~PSocketServer();
	virtual void Run();
    virtual void Start();

protected:
    virtual void Initialize();
	virtual void Finalize();

    virtual void Pause();
    virtual void Stop();
};