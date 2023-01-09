#pragma once
#include "../TypeDef.h"
#define Component_Id _GET_CLASS_UID(_EPlatform::_eComponent) 
#define Component_Name "Component"

#include "UId.h"
#include "../../1Base/StdLib/Vector.h"
#include "../../1Base/\Aspect/Directory.h"

class IComponent {
public:
	enum class EEventType {
		eBegin = Component_Id,

		eAssociateAReceiver,
		eAssociateATarget,
		eInitialize,
		eFinalize,

		eStart,
		eStop,

		eRun,
		ePause,


		eEnd
	};

	enum class EParts {
		eBegin = Component_Id,
		eEnd
	};
	enum class EReceivers {
		eBegin = Component_Id,
		eThis,
		eLifecycleManager,
		eScheduler,
		eExceptionManager,
		eLogManager,
		eEnd
	};
	enum class ETargetGroups {
		eBegin = Component_Id,
		eEnd
	};
	enum class EAttributes {
		eBegin = Component_Id,
		eEnd
	};

	class ParamAssociateAReceiver : public BaseObject {
	private:
		unsigned m_uReceiverName;
		UId m_uIdReceiverComponent;
	public:
		ParamAssociateAReceiver(unsigned uReceiverName, UId uIdReceiverComponent)
			: m_uReceiverName(uReceiverName)
			, m_uIdReceiverComponent(uIdReceiverComponent)
		{}
		~ParamAssociateAReceiver() {}
		unsigned GetReceiverName() { return this->m_uReceiverName; }
		UId GetUIdReceiverComponent() { return this->m_uIdReceiverComponent; }
	};

	class ParamAssociateATarget : public BaseObject {
	private:
		unsigned m_uGroupName;
		Vector<UId> m_vUIdTargetComponents;

	public:
		ParamAssociateATarget(unsigned uGroupName)
			: m_uGroupName(uGroupName)
		{}
		~ParamAssociateATarget() {}

		unsigned GetGroupName() { return this->m_uGroupName; }
		Vector<UId>& GetVUIdTargetComponents() { return this->m_vUIdTargetComponents; }
	};
};
