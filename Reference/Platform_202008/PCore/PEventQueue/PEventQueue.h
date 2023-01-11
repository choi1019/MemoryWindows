#pragma once
#include "../TypeDef.h"
#define PEVENTQUEUE "PEventQueue"  // class string name
#define PEVENTQUEUE_ID GET_MODULE_ID(EPCore::ePEventQueue)  // class int id

#include "../../../../Core_Lib/Source/03Core/EventQueue/EventQueue.h"
/* FreeRTOS includes. */
#include "../../../../FreeRTOS_Lib/Source/include/FreeRTOS.h"
#include "../../../../FreeRTOS_Lib/Source/include/queue.h"

class PEventQueue : public EventQueue {
private:
	QueueHandle_t xQueue;
public:
	PEventQueue() {
		/* The queue is created to hold a maximum of 5 long values. */
		this->xQueue = xQueueCreate(MAXQUEUESIZE, sizeof(Event *));

		if (xQueue == NULL) {

		}
	}

	virtual ~PEventQueue() {
	} 

	virtual unsigned size() {
		return uxQueueMessagesWaiting(this->xQueue);
	}
	Event* pop() {
		Event* pEvent = nullptr;
		BaseType_t xStatus = xQueueReceive(this->xQueue, &pEvent, portMAX_DELAY);
		return pEvent;
	}

	void push(Event* pEvent) {
		BaseType_t xStatus = xQueueSendToBack(this->xQueue, &pEvent, portMAX_DELAY);
	}
};