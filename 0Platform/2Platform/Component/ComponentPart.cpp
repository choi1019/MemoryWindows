#include "ComponentPart.h"

#include "../../1Base/Aspect/Directory.h"
#include "../../1Base/StdLib/String.h"
#include "../../2Platform/EventQueue/EventQueue.h"

ComponentPart::ComponentPart(unsigned uClassId, const char* acClassName)
	: BaseObject(uClassId, acClassName)
	, m_pUId(nullptr)
	, m_pmReceivers(nullptr)
	, m_pmTargetsGroups(nullptr)
	, m_pSequenceHead(nullptr)
	, m_pSequenceParent(nullptr) 
{
	this->RegisterExceptions();
}

ComponentPart::~ComponentPart() {
}

void ComponentPart::Initialize() {
	BaseObject::Initialize();
}

void ComponentPart::Finalize() {
	BaseObject::Finalize();
}

void ComponentPart::RegisterExceptions() {
	Directory::s_dirExceptions[(unsigned)EException::eNotAllocated] = "eNotAllocated";
	Directory::s_dirExceptions[(unsigned)EException::eReceiverNotFound] = "eReceiverNotFound";
	Directory::s_dirExceptions[(unsigned)EException::eNotAssociated] = "eNotAssociated";
	Directory::s_dirExceptions[(unsigned)EException::eNotTargeted] = "eNotTargeted";
	Directory::s_dirExceptions[(unsigned)EException::eEventNotSupported] = "eEventNotSupported";
}

UId ComponentPart::FindUid(int nReceiverName) {
	auto iterator = this->m_pmReceivers->Find(nReceiverName);
	if (iterator == m_pmReceivers->end()) {
		throw Exception(
			(unsigned)EException::eNotAssociated,
			this->GetClassName(),
			__func__,
			String("Receiver(") + String(nReceiverName) + ") is not Associated"
		);
	}
	return iterator->second;
}

// for transaction
void ComponentPart::BeginSequence(Event* pEvent) {
	m_pSequenceHead = nullptr;
	m_pSequenceParent = pEvent;
}
void ComponentPart::EndSequence() {
	for (; m_pSequenceHead != nullptr; m_pSequenceHead = m_pSequenceHead->GetPNextSequence()) {
//		LOG("ComponentPart::EndSequence",
//			Directory::s_dirComponents[m_pSequenceHead->GetUIdTarget().GetComponentId()].c_str(),
//			Directory::s_dirEvents[m_pSequenceHead->GetType()].c_str()
//		);
		if (m_pSequenceHead->GetESendOrder() == Event::ESendOrder::eFirst) {
			m_pSequenceHead->GetUIdTarget().GetPEventQueue()->PushFront(m_pSequenceHead);
		}
		else {
			m_pSequenceHead->GetUIdTarget().GetPEventQueue()->PushBack(m_pSequenceHead);
		}
	}
}

///////////////////////////////////////
// prepare event
///////////////////////////////////////
void ComponentPart::SendAEvent(Event* pEvent) {
	if (pEvent->GetUIdTarget().GetPEventQueue() == nullptr) {
		throw Exception(
			(unsigned)EException::eNotAllocated,
			this->GetClassName(),
			__func__,
			"EventQueue is not allocated"
		);
	}
	// insert current event at the head
	if (m_pSequenceHead == nullptr) {
		pEvent->SetPNextSequence(nullptr);
		m_pSequenceHead = pEvent;
	}
	else {
		pEvent->SetPNextSequence(m_pSequenceHead);
		m_pSequenceHead = pEvent;
	}

	// for reply
	if (pEvent->GetESyncType() == Event::ESyncType::eSync) {
		pEvent->SetPParentEvent(m_pSequenceParent);
		if (m_pSequenceParent != nullptr) {
			m_pSequenceParent->IncrementCountChildren();
			if (pEvent->GetReplyType() == UNDEFINED) {
				pEvent->SetReplyType(m_pSequenceParent->GetType());
			}
		}
	}
}

///////////////////////////////////////
// reply event
///////////////////////////////////////
void ComponentPart::PrepareReplyEvent(Event* pEvent) {
	// swap source and destination
	UId uIdSource = pEvent->GetUIdSource();
	pEvent->SetUIdSource(pEvent->GetUIdTarget());
	pEvent->SetUIdTarget(uIdSource);

	// swap event type and reply event type
	int nReplyEventType = pEvent->GetReplyType();
	pEvent->SetReplyType(pEvent->GetType());
	pEvent->SetType(nReplyEventType);

	if (pEvent->GetPParentEvent() != nullptr) {
		pEvent->GetPParentEvent()->DecrementCountChildren();
	}
}

///////////////////////////////////////
// synchronous event
///////////////////////////////////////
void ComponentPart::SendReplyEvent(UId uIdTarget, int nEventType, long long lArg, BaseObject* pArg, int nReplyType)
{
	Event* pEvent = new Event(this->GetUId(), uIdTarget, nEventType, lArg, pArg
		, nReplyType, nullptr, Event::ESyncType::eSync);
	this->SendAEvent(pEvent);
}
void ComponentPart::SendReplyEvent(int nReceiverName, int nEventType, long long lArg, BaseObject* pArg, int nReplyType)
{
	UId uIdTarget = this->FindUid(nReceiverName);
	this->SendReplyEvent(uIdTarget, nEventType, lArg, pArg, nReplyType);
}


///////////////////////////////////////
// asynchronous event
///////////////////////////////////////
void ComponentPart::SendNoReplyEvent(UId uIdTarget, int nEventType, long long lArg, BaseObject* pArg)
{
	Event* pEvent = new Event(this->GetUId(), uIdTarget, nEventType, lArg, pArg
		, UNDEFINED, nullptr, Event::ESyncType::eAsync);
	this->SendAEvent(pEvent);
}
void ComponentPart::SendNoReplyEvent(int nReceiverName, int nEventType, long long lArg, BaseObject* pArg)
{
	UId uIdTarget = this->FindUid(nReceiverName);
	this->SendNoReplyEvent(uIdTarget, nEventType, lArg, pArg);
}

void ComponentPart::SendNoReplyEventLast(UId uIdTarget, int nEventType, long long lArg, BaseObject* pArg)
{
	Event* pEvent = new Event(this->GetUId(), uIdTarget, nEventType, lArg, pArg
		, UNDEFINED, nullptr, Event::ESyncType::eAsync, Event::ESendOrder::eLast);
	this->SendAEvent(pEvent);
}
void ComponentPart::SendNoReplyEventLast(int nReceiverName, int nEventType, long long lArg, BaseObject* pArg)
{
	UId uIdTarget = this->FindUid(nReceiverName);
	this->SendNoReplyEventLast(uIdTarget, nEventType, lArg, pArg);
}

///////////////////////////////////////
// target events
///////////////////////////////////////
void ComponentPart::SendTargetEvents(unsigned groupName, unsigned eventType, long long lArg, BaseObject* pArg) {
	for (auto& itr : *(this->m_pmTargetsGroups->Find(groupName)->second))
	{
		this->SendNoReplyEvent(itr, eventType, lArg, pArg);
	}
}

void ComponentPart::SendTargetEventsLast(unsigned groupName, unsigned eventType, long long lArg, BaseObject* pArg) {
	for (auto& itr : *(this->m_pmTargetsGroups->Find(groupName)->second))
	{
		this->SendNoReplyEventLast(itr, eventType, lArg, pArg);
	}
}

