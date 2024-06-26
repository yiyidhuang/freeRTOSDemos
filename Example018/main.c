
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

#include "supporting_functions.h"

#define mainINTERRUPT_NUMBER        3

static void vPeriodicTask(void* pvParameters);

static void vDeferredHandlingFunction(void* pvParameter1, uint32_t ulParameter2);

static uint32_t ulExampleInterruptHandler(void);

int main(void)
{
    const UBaseType_t ulPeriodicTaskPriority = configTIMER_TASK_PRIORITY - 1;

    xTaskCreate(vPeriodicTask, "Periodic", 1000, NULL, ulPeriodicTaskPriority, NULL);

    vPortSetInterruptHandler(mainINTERRUPT_NUMBER, ulExampleInterruptHandler);

    vTaskStartScheduler();

    for (;;);

    return 0;
}

static void vPeriodicTask(void* pvParameters)
{
    const TickType_t xDelay500ms = pdMS_TO_TICKS(500UL);

    for (;;) {
        vTaskDelay(xDelay500ms);

        vPrintString("Periodic task - About to generate an interrupt.\r\n");
        vPortGenerateSimulatedInterrupt(mainINTERRUPT_NUMBER);
        vPrintString("Periodic task - Interrupt generated.\r\n\r\n\r\n");
    }
}

static uint32_t ulExampleInterruptHandler(void)
{
    static uint32_t ulParameterValue = 0;
    BaseType_t xHigherPriorityTaskWoken;

    xHigherPriorityTaskWoken = pdFALSE;

    xTimerPendFunctionCallFromISR(vDeferredHandlingFunction, NULL, ulParameterValue, &xHigherPriorityTaskWoken);
    ulParameterValue++;

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

static void vDeferredHandlingFunction(void* pvParameter1, uint32_t ulParameter2)
{
    (void)pvParameter1;

    vPrintStringAndNumber("Handler Function - Processing event", ulParameter2);
}
