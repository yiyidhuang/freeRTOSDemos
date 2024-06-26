
#include <stdio.h>
#include <conio.h>

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "supporting_functions.h"

static void prvPrintTask(void* pvParameters);

static void prvStdioGatekeeperTask(void* pvParameters);

static const char* pcStringsToPrint[] =
{
    "Task 1 ****************************************************\r\n",
    "Task 2 ----------------------------------------------------\r\n",
    "Message printed from the tick hook interrupt ##############\r\n"
};

static QueueHandle_t xPrintQueue;

const TickType_t xMaxBlockTimeTicks = 0x20;

int main(void)
{
    xPrintQueue = xQueueCreate(5, sizeof(char*));

    if (xPrintQueue != NULL) {
        xTaskCreate(prvPrintTask, "Print1", 1000, (void*)0, 1, NULL);
        xTaskCreate(prvPrintTask, "Print2", 1000, (void*)1, 2, NULL);

        xTaskCreate(prvStdioGatekeeperTask, "Gatekeeper", 1000, NULL, 0, NULL);

        vTaskStartScheduler();
    }

    for (;;);

    return 0;
}

static void prvStdioGatekeeperTask(void* pvParameters)
{
    char* pcMessageToPrint;

    for (;;) {
        xQueueReceive(xPrintQueue, &pcMessageToPrint, portMAX_DELAY);

        printf("%s", pcMessageToPrint);

        fflush(stdout);
    }
}

void vApplicationTickHook(void)
{
    static int iCount = 0;

    iCount++;
    if (iCount >= 200) {
        xQueueSendToFrontFromISR(xPrintQueue, &(pcStringsToPrint[2]), NULL);

        iCount = 0;
    }
}

static void prvPrintTask(void* pvParameters)
{
    int iIndexToString;

    iIndexToString = (int)pvParameters;

    for (;;) {
        xQueueSendToBack(xPrintQueue, &(pcStringsToPrint[iIndexToString]), 0);
        vTaskDelay((rand() % xMaxBlockTimeTicks));
    }
}

void vAssertCalled(uint32_t ulLine, const char* const pcFile)
{
    volatile uint32_t ulLineNumber = ulLine, ulSetNonZeroInDebuggerToReturn = 0;
    volatile const char* const pcFileName = pcFile;

    taskENTER_CRITICAL();
    {
        while (ulSetNonZeroInDebuggerToReturn == 0) {

        }
    }
    taskEXIT_CRITICAL();

    (void)pcFileName;
    (void)ulLineNumber;
}

void vApplicationMallocFailedHook(void)
{
    vAssertCalled(__LINE__, __FILE__);
}
