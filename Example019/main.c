#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "supporting_functions.h"

#define mainINTERRUPT_NUMBER        3

static void vIntegerGenerator(void* pvParameters);
static void vStringPrinter(void* pvParameters);

static uint32_t ulExampleInterruptHandler(void);

QueueHandle_t xIntegerQueue, xStringQueue;

int main(void)
{
    xIntegerQueue = xQueueCreate(10, sizeof(uint32_t));
    xStringQueue = xQueueCreate(10, sizeof(char*));

    xTaskCreate(vIntegerGenerator, "IntGen", 1000, NULL, 1, NULL);

    xTaskCreate(vStringPrinter, "String", 1000, NULL, 2, NULL);

    vPortSetInterruptHandler(mainINTERRUPT_NUMBER, ulExampleInterruptHandler);

    vTaskStartScheduler();

    for (;;);

    return 0;
}

static void vIntegerGenerator(void* pvParameters)
{
    TickType_t xLastExecutionTime;
    const TickType_t xDelay200ms = pdMS_TO_TICKS(200UL), xDontBlock = 0;
    uint32_t ulValueToSend = 0;
    BaseType_t i;

    xLastExecutionTime = xTaskGetTickCount();

    for (;;) {
        vTaskDelayUntil(&xLastExecutionTime, xDelay200ms);

        for (i = 0; i < 5; i++) {
            xQueueSendToBack(xIntegerQueue, &ulValueToSend, xDontBlock);
            ulValueToSend++;
        }

        vPrintString("Generator task - About to generate an interrupt.\r\n");
        vPortGenerateSimulatedInterrupt(mainINTERRUPT_NUMBER);
        vPrintString("Generator task - Interrupt generated.\r\n\r\n\r\n");
    }
}

static void vStringPrinter(void* pvParameters)
{
    char* pcString;

    for (;;) {
        xQueueReceive(xStringQueue, &pcString, portMAX_DELAY);

        vPrintString(pcString);
    }
}

static uint32_t ulExampleInterruptHandler(void)
{
    BaseType_t xHigherPriorityTaskWoken;
    uint32_t ulReceivedNumber;

    static const char* pcStrings[] = {
        "String 0\r\n",
        "String 1\r\n",
        "String 2\r\n",
        "String 3\r\n",
    };

    xHigherPriorityTaskWoken = pdFALSE;

    while (xQueueReceiveFromISR(xIntegerQueue, &ulReceivedNumber, &xHigherPriorityTaskWoken) != errQUEUE_EMPTY) {
        ulReceivedNumber &= 0x03;
        xQueueSendToBackFromISR(xStringQueue, &pcStrings[ulReceivedNumber], &xHigherPriorityTaskWoken);
    }

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
