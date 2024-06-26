
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "supporting_functions.h"

#define mainINTERRUPT_NUMBER    3

static void vHanderTask(void* pvParameters);
static void vPeriodicTask(void* pvParameters);

static uint32_t ulExampleInterruptHandler(void);

static const TickType_t xInterruptFrequency = pdMS_TO_TICKS(500UL);

static TaskHandle_t xHandlerTask = NULL;

int main(void)
{
    xTaskCreate(vHanderTask, "Handler", 1000, NULL, 3, &xHandlerTask);
    xTaskCreate(vPeriodicTask, "Periodic", 1000, NULL, 1, NULL);

    vPortSetInterruptHandler(mainINTERRUPT_NUMBER, ulExampleInterruptHandler);

    vTaskStartScheduler();

    for (;;);

    return 0;
}

static void vHanderTask(void* pvParameters)
{
    const TickType_t xMaxExpectedBlockTime = xInterruptFrequency + pdMS_TO_TICKS(10);

    for (;;) {
        if (ulTaskNotifyTake(pdFALSE, xMaxExpectedBlockTime) != 0) {
            vPrintString("Handler task - Processing event.\r\n");
        }
        else {

        }
    }
}

static uint32_t ulExampleInterruptHandler(void)
{
    BaseType_t xHigherPriorityTaskWoken;

    xHigherPriorityTaskWoken = pdFALSE;

    vTaskNotifyGiveFromISR(xHandlerTask, &xHigherPriorityTaskWoken);
    vTaskNotifyGiveFromISR(xHandlerTask, &xHigherPriorityTaskWoken);
    vTaskNotifyGiveFromISR(xHandlerTask, &xHigherPriorityTaskWoken);

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

static void vPeriodicTask(void* pvParameters)
{
    for (;;) {
        vTaskDelay(xInterruptFrequency);

        vPrintString("Periodic task - About to generate an interrupt.\r\n");
        vPortGenerateSimulatedInterrupt(mainINTERRUPT_NUMBER);
        vPrintString("Periodic task - Interrupt generated.\r\n\r\n\r\n");
    }
}
