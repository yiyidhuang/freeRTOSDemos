﻿
#include "FreeRTOS.h"
#include "task.h"

#include "supporting_functions.h"

void vTaskFunction(void* pvParameters);

const char* pcTextForTask1 = "Task 1 is running\r\n";
const char* pcTextForTask2 = "Task 2 is running\r\n";

int main(void)
{
    xTaskCreate(vTaskFunction, "Task 1", 1000, (void*)pcTextForTask1, 1, NULL);

    xTaskCreate(vTaskFunction, "Task 2", 1000, (void*)pcTextForTask2, 2, NULL);

    vTaskStartScheduler();

    for (;;);

    return 0;
}

void vTaskFunction(void* pvParameters)
{
    char* pcTaskName;
    TickType_t xLastWakeTime;
    const TickType_t xDelay250ms = pdMS_TO_TICKS(250UL);

    pcTaskName = (char*)pvParameters;

    xLastWakeTime = xTaskGetTickCount();

    for (;;) {
        vPrintString(pcTaskName);

        vTaskDelayUntil(&xLastWakeTime, xDelay250ms);
    }
}
