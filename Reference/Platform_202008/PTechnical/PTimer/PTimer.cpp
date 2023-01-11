#include "PTimer.h"

Timer *Timer::vectorPPTimer[MAX_TIMER_COUNT];

static void _TimerAPCProc(TimerHandle_t xTimer)
{
    uint32_t timerId = (uint32_t)pvTimerGetTimerID(xTimer);
    Timer::vectorPPTimer[timerId]->timesUp(); 
}

uint32_t PTimer::sId = 0;

PTimer::PTimer(unsigned dueTime, unsigned interval, ClassInfo classInfo):
    xAutoReloadTimer(nullptr), xTimerStarted(pdFAIL),
    PScheduler(classInfo), Timer(dueTime, interval, classInfo)
{
    vectorPPTimer[sId] = this;

    /* Create the auto-reload timer, storing the handle to the created timer in xAutoReloadTimer. */
    xAutoReloadTimer = xTimerCreate(
        /* Text name for the software timer - not used by FreeRTOS. */
        "AutoReload",
        /* The software timer's period in ticks. */
        pdMS_TO_TICKS(this->interval),
        /* Setting uxAutoRealod to pdTRUE creates an auto-reload timer. */
        pdTRUE,
        /* This example does not use the timer id. */
        (void *)sId++,
        /* The callback function to be used by the software timer being created. */
        _TimerAPCProc
    );
    /* Check the software timers were created. */
    if (xAutoReloadTimer == NULL) {
 //       cout << "Error" << _methodName() << _endl;
    }
}

PTimer::~PTimer() {
}

void PTimer::initialize() {
    this->xTimerStarted = xTimerStart(this->xAutoReloadTimer, this->dueTime);

    /* The implementation of xTimerStart() uses the timer command queue, and
    xTimerStart() will fail if the timer command queue gets full.  The timer
    service task does not get created until the scheduler is started, so all
    commands sent to the command queue will stay in the queue until after
    the scheduler has been started.  Check both calls to xTimerStart()
    passed.
    */
    if (this->xTimerStarted == pdPASS)
    {
//        cout << "PTimer started" << "\n";
    }
}

void PTimer::finalize() {

}


