#pragma once

#include "../../1Base/StdLib/Map.h"
#include "../../1Base/StdLib/Vector.h"
#include "ILifecycleManager.h"

class LifecycleManager : public ILifecycleManager, public Component{

private:
	// <schedulerName, schedulerPtr>
	typedef Map<int, Scheduler*> MapSchedulers;
	// <componentName, ComponentPtr>
	typedef Map<int, Component*> MapComponents;
	// <componentName, schedulerName>
	typedef Map<int, int> MapAllocations;
	// <senderName, local name in sender>, ReceiverName
	typedef MapPair<int, unsigned> MapPairSender;
	typedef Map<MapPairSender, int> MapSendersNReceivers;
	// <sourceName, groupName>, vector<tarGetName>*> SourcesNTargetsMap
	typedef MapPair<int, unsigned> MapPairSource;
	typedef Vector<int> VectorTargetNames;
	typedef Map<MapPairSource, VectorTargetNames*> MapSourcesNTargets;

	MapSchedulers m_mapSchedulers;
	MapComponents m_mapComponents;
	MapAllocations m_mapAllocations;
	MapSendersNReceivers m_mapSendersNReceivers;
	MapSourcesNTargets	m_mapSourcesNTargets;


	// calback pointer to PMain
	Scheduler* m_pMainScheduler;

	void RegisterEventTypes();
	void RegisterExceptions();

	void InitializeVariables();
	void DeleteComponents();

	Scheduler* FindAScheduler(UId uId);
	Component* FindAComponent(UId uId);

public:
	// constructors & destructors
	LifecycleManager(int nClassId = _LifecycleManager_Id, const char* pcClassName = _LifecycleManager_Name);
	virtual ~LifecycleManager();

protected:
	virtual void Initialize();
	virtual void Finalize();

protected:
	void RegisterAScheduler(int name, Scheduler* pScheduler);
	void RegisterAComponent(int name, Component* pComponent);
	void AllocateAComponent(int componentName, int schedulerName);
	void AssociateASenderNAReceiver(int senderName, int receiverId, int receiverName);
	// <sourceName, source.GroupName + vector<tarGetName>*
	void AssociateASourceNATarget(int nSourceName, int nGroupId, int nTarGetName);

	/////////////////////////////////////////////////////////////////////////
	virtual void RegisterSystemSchedulers();
	virtual void RegisterUserShedulers() = 0;
	/////////////////////////////////////////////////////////////////////////
	virtual void RegisterSystemComponents();
	virtual void RegisterUserComponents() = 0;
	/////////////////////////////////////////////////////////////////////////
	virtual void AllocateSystemComponents();
	virtual void AllocateUserComponents() = 0;
	/////////////////////////////////////////////////////////////////////////
	virtual void AssociateSystemSendersNReceivers();
	virtual void AssociateUserSendersNReceivers() = 0;
	//////////////////////////////////////////////////////////////////////////
	virtual void AssociateSystemSourcesNTargets();
	virtual void AssociateUserSourcesNTargets() = 0;
	//////////////////////////////////////////////////////////////////////////
	virtual void StartComponents() = 0;
	//////////////////////////////////////////////////////////////////////////

private: 
	///////////////////////////////////////////////////
	// InitializeAsALifecycleManager
	///////////////////////////////////////////////////
	//  InitializeAsALifecycleManager
	void InitializeAsALifecycleManager(Event* pEvent);

	// Register Schedulers
	void RegisterSchedulers(Event* pEvent);
	// Initialize Schedulers
	void InitializeSchedulers(Event* pEvent);
	// Start Schedulers
	void StartSchedulers(Event* pEvent);
	// Register Components
	void RegisterComponents(Event* pEvent);
	// Allocate Components
	void AllocateComponents(Event* pEvent);
	// associate senders and recievers
	void AssociateSendersNReceivers(Event* pEvent);
	// associate a source and targets
	void AssociateSourcesNTargets(Event* pEvent);
	// Initialize Components
	void InitializeComponents(Event* pEvent);
	//  start Components
	void StartComponents(Event* pEvent);

private:
	// Stop System
	void StopSystem(Event* pEvent);

	///////////////////////////////////////////////////
	// FinalizeAsALifecycleManager
	///////////////////////////////////////////////////
	void FinalizeComponents(Event* pEvent);
	void StopSchedulers(Event* pEvent);
	void FinalizeSchedulers(Event* pEvent);
	void FinalizeAsALifecycleManager(Event* pEvent);

protected:
	virtual void ProcessAEvent(Event* pEvent);
};
