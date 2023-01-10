#pragma once
#include "../typedef.h"
#define _RSTP_Id _GET_CLASS_UID(_ELayer_PTechnical::_eRSTP)
#define _RSTP_Name "RSTP"

#include "../../3Technical/Protocol/Protocol.h"

class RSTP : public Protocol {
public:
    enum class EEventType {
        eBegin = _RSTP_Id,
        eOptions,
        eDescribe,
        eAnnounce,
        eSetup,
        ePlay,
        ePause,
        eRecord,
        eTeardown,
        eRedirect,
        eEnd
    };

    RSTP() : Protocol() {}
    virtual ~RSTP() {}

    void Option() {}
    void Describe() {}
    void Announce() {}
    void Setup() {}
    void Play() {}
    void Pause() {}
    void Record() {}
    void Teardown() {}
    void Redirect() {}

protected:
    virtual void Initialize() {}
    virtual void Finalize() {}
};

