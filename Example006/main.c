
#include "FreeRTOS.h"
#include "task.h"

#include "supporting_functions.h"

void vContinuousProcessingTask(void* pvParameters);
void vPeriodicTask(void* pvParameters);

const char* pcTextForTask1 = "Continuous task 1 running\r\n";
const char* pcTextForTask2 = "Continuous task 2 running\r\n";
const char* pcTextForPeriodicTask = "Periodic task is running\r\n";

int main(void)
{
    xTaskCreate(vContinuousProcessingTask, "Task 1", 1000, (void*)pcTextForTask1, 1, NULL);
    xTaskCreate(vContinuousProcessingTask, "Task 2", 1000, (void*)pcTextForTask2, 1, NULL);

    xTaskCreate(vPeriodicTask, "Task 3", 1000, (void*)pcTextForPeriodicTask, 2, NULL);

    vTaskStartScheduler();

    for (;;);

    return 0;
}

void vContinuousProcessingTask(void* pvParameters)
{
    char* pcTaskName;

    pcTaskName = (char*)pvParameters;

    for (;;) {
        vPrintString(pcTaskName);
    }
}

void vPeriodicTask(void* pvParameters)
{
    TickType_t xLastWakeTime;
    const TickType_t xDelay5ms = pdMS_TO_TICKS(3UL);

    xLastWakeTime = xTaskGetTickCount();

    for (;;) {
        vPrintString("Periodic task is running\r\n");

        vTaskDelayUntil(&xLastWakeTime, xDelay5ms);
    }
}
