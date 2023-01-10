#pragma once
#include "../typedef.h"
#define _Protocol_Id _GET_CLASS_UID(_ELayer_PTechnical::_eProtocol)
#define _Protocol_Name "Protocol"

#include "../../1Base/Object/BaseObject.h"
class Protocol : public BaseObject {
public:
    Protocol() : BaseObject() {}
    virtual ~Protocol() {}

    virtual void Parse() {}
    virtual void Generate() {}
protected:
    virtual void Initialize() {}
    virtual void Finalize() {}
};

