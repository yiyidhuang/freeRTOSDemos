
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "supporting_functions.h"

#define mainINTERRUPT_NUMBER 3

static void vHandlerTask(void* pvParameters);
static void vPeriodicTask(void* pvParameters);

static uint32_t ulExampleInterruptHandler(void);

SemaphoreHandle_t xCountingSemaphore;

int main(void)
{
    xCountingSemaphore = xSemaphoreCreateCounting(10, 0);

    if (xCountingSemaphore != NULL) {
        xTaskCreate(vHandlerTask, "Handler", 1000, NULL, 3, NULL);

        xTaskCreate(vPeriodicTask, "Periodic", 1000, NULL, 1, NULL);

        vPortSetInterruptHandler(mainINTERRUPT_NUMBER, ulExampleInterruptHandler);

        vTaskStartScheduler();
    }

    for (;;);

    return 0;
}

static void vHandlerTask(void* pvParameters)
{
    for (;;) {
        xSemaphoreTake(xCountingSemaphore, portMAX_DELAY);

        vPrintString("Handler task - Processing event. \r\n");
    }
}

static void vPeriodicTask(void* pvParameters)
{
    const TickType_t xDelay500ms = pdMS_TO_TICKS(500UL);

    for (;;) {
        vTaskDelay(xDelay500ms);

        vPrintString("Periodic task - About to generate an interrupt. \r\n");
        vPortGenerateSimulatedInterrupt(mainINTERRUPT_NUMBER);
        vPrintString("Periodic task - Interrupt generated. \r\n\r\n\r\n");
    }
}

static uint32_t ulExampleInterruptHandler(void)
{
    BaseType_t xHigherPriorityTaskWoken;

    xHigherPriorityTaskWoken = pdFALSE;

    xSemaphoreGiveFromISR(xCountingSemaphore, &xHigherPriorityTaskWoken);
    xSemaphoreGiveFromISR(xCountingSemaphore, &xHigherPriorityTaskWoken);
    xSemaphoreGiveFromISR(xCountingSemaphore, &xHigherPriorityTaskWoken);

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
