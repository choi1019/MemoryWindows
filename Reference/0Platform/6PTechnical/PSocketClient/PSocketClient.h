#pragma once

#include "../typedef.h"
#define _PSocketClient_Id _GET_CLASS_UID(_ELayer_PTechnical::_ePSocketClient)
#define _PSocketClient_Name "PSocketClient"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "../../3Technical/SocketClient/SocketClient.h"
#include "../../2Platform/Component/Component.h"

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

class PSocketClient : public Component, public SocketClient
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
    PSocketClient(int nClassId = _PSocketClient_Id, const char* pcClassName = _PSocketClient_Name);
    virtual ~PSocketClient();
	virtual void Run();

protected:
    virtual void Initialize();
	virtual void Finalize();

    virtual void Start();
    virtual void Stop();
    virtual void Pause();
};