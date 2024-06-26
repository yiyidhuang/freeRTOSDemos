
#include <conio.h>

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

#include "supporting_functions.h"

#define mainBACKLIGHT_TIMER_PERIOD          (pdMS_TO_TICKS(5000UL))

static void prvBacklightTimerCallback(TimerHandle_t xTimer);

static void vKeyHitTask(void* pvParameters);

static BaseType_t xSimulateBacklightOn = pdFALSE;

static TimerHandle_t xBacklightTimer = NULL;

int main(void)
{
    xSimulateBacklightOn = pdFALSE;

    xBacklightTimer = xTimerCreate("Backlight",
        mainBACKLIGHT_TIMER_PERIOD,
        pdFALSE,
        0,
        prvBacklightTimerCallback);

    xTaskCreate(vKeyHitTask,
        "key poll",
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY,
        NULL);

    xTimerStart(xBacklightTimer, 0);

    vTaskStartScheduler();

    for (;;);

    return 0;
}

static void prvBacklightTimerCallback(TimerHandle_t xTimer)
{
    TickType_t xTimeNow = xTaskGetTickCount();

    xSimulateBacklightOn = pdFALSE;

    vPrintStringAndNumber("Timer expired, turning backlight OFF at time\t", xTimeNow);
}

static void vKeyHitTask(void* pvParameters)
{
    const TickType_t xShortDelay = pdMS_TO_TICKS(50);
    extern BaseType_t xKeyPressesStopApplication;
    TickType_t xTimeNow;

    xKeyPressesStopApplication = pdFALSE;

    vPrintString("Press a key to turn the backlight on. \r\n");

    for (;;) {
        if (_kbhit() != 0) {
            xTimeNow = xTaskGetTickCount();

            if (xSimulateBacklightOn == pdFALSE) {
                xSimulateBacklightOn = pdTRUE;
                vPrintStringAndNumber("Key pressed, truning backlight ON at time\t",
                    xTimeNow);
            }
            else {
                vPrintStringAndNumber("Key pressed, resetting softwares timer at time\t",
                    xTimeNow);
            }

            xTimerReset(xBacklightTimer, xShortDelay);

            (void)_getch();
        }

        vTaskDelay(xShortDelay);
    }
}
