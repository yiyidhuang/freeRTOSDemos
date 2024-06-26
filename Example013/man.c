﻿
#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

#include "supporting_functions.h"

#define mainONE_SHOT_TIMER_PERIOD       (pdMS_TO_TICKS(3333UL))
#define mainAUTO_RELOAD_TIMER_PERIOD    (pdMS_TO_TICKS(500UL))

static void prvOneShotTimerCallback(TimerHandle_t xTimer);
static void prvAutoReloadTimerCallback(TimerHandle_t xTimer);

int main(void)
{
    TimerHandle_t xAutoReloadTimer, xOneShotTimer;
    BaseType_t xTimer1Started, xTimer2Started;

    xOneShotTimer = xTimerCreate("OneShot",
        mainONE_SHOT_TIMER_PERIOD,
        pdFALSE,
        0,
        prvOneShotTimerCallback);

    xAutoReloadTimer = xTimerCreate("AutoReload",
        mainAUTO_RELOAD_TIMER_PERIOD,
        pdTRUE,
        0,
        prvAutoReloadTimerCallback);

    if ((xOneShotTimer != NULL) && (xAutoReloadTimer != NULL)) {
        xTimer1Started = xTimerStart(xOneShotTimer, 0);
        xTimer2Started = xTimerStart(xAutoReloadTimer, 0);

        if ((xTimer1Started == pdPASS) && (xTimer2Started == pdPASS)) {
            vTaskStartScheduler();
        }
    }

    for (;;);

    return 0;
}

static void prvOneShotTimerCallback(TimerHandle_t xTimer)
{
    static TickType_t xTimeNow;

    xTimeNow = xTaskGetTickCount();

    vPrintStringAndNumber("One-shot timer callback executing", xTimeNow);
}

static void prvAutoReloadTimerCallback(TimerHandle_t xTimer)
{
    static TickType_t xTimeNow;

    xTimeNow = xTaskGetTickCount();

    vPrintStringAndNumber("Auto-reload timer callback executing", xTimeNow);
}
