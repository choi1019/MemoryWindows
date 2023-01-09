
#include "../typedef.h"
#define _PLifecycleManager_Id _GET_CLASS_UID(_EPApplication::_ePLifecycleManager)
#define _PLifecycleManager_name "PLifecycleManager"

#include "../../../Platform/2Platform/LifecycleManager/LifecycleManager.h"

class PLifecycleManager : public LifecycleManager {
public:
	////////////////////////////////////////
	// Required interface
	////////////////////////////////////////
	enum class EComponents {
		eBegin = _PLifecycleManager_Id,
		eScheduler1,
		eScheduler2,
		eTimer,

		eSocketClient,

		eSender,
		eReceiver,
		eSource,
		eTarget1,
		eTarget2,
		eEnd
	};
public:
	PLifecycleManager(
		unsigned typeId = _PLifecycleManager_Id,
		const char* pClassName = _PLifecycleManager_name);
	virtual ~PLifecycleManager();

protected:
	virtual void RegisterUserShedulers();
	virtual void RegisterUserComponents();
	virtual void AllocateUserComponents();
	virtual void AssociateUserSendersNReceivers();
	virtual void AssociateUserSourcesNTargets();

	virtual void Initialize();
	virtual void Finalize();

	virtual void StartComponents();
	virtual void StopComponents();
};
