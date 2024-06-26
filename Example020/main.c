
#include <stdio.h>
#include <conio.h>

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "supporting_functions.h"


static void prvPrintTask(void* pvParameters);

static void prvNewPrintString(const char* pcString);

SemaphoreHandle_t xMutex;

const TickType_t xMaxBlockTimeTicks = 0x20;

int main(void)
{
    xMutex = xSemaphoreCreateMutex();

    if (xMutex != NULL) {
        xTaskCreate(prvPrintTask,
            "Print1",
            1000,
            "Task 1 ******************************************\r\n",
            1,
            NULL);

        xTaskCreate(prvPrintTask,
            "Print2",
            1000,
            "Task 2 ------------------------------------------\r\n",
            2,
            NULL);

        vTaskStartScheduler();
    }

    for (;;);

    return 0;
}

static void prvNewPrintString(const char* pcString)
{
    xSemaphoreTake(xMutex, portMAX_DELAY);
    {
        printf("%s", pcString);
        fflush(stdout);
    }
    xSemaphoreGive(xMutex);

    if (_kbhit() != 0) {
        vTaskEndScheduler();
    }
}

static void prvPrintTask(void* pvParameters)
{
    char* pcStringToPrint;
    const TickType_t xSlowDownDelay = pdMS_TO_TICKS(5UL);

    pcStringToPrint = (char*)pvParameters;

    for (;;) {
        prvNewPrintString(pcStringToPrint);

        vTaskDelay(rand() % xMaxBlockTimeTicks);

        vTaskDelay(xSlowDownDelay);
    }
}
