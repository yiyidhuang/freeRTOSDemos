
#include "FreeRTOS.h"
#include "task.h"

#include "supporting_functions.h"

#define mainDELAY_LOOP_COUNT    (0xffffff)

void vTaskFunction(void* pvParameters);

const char* pcTextForTask1 = "Task 1 is running\r\n";
const char* pcTextForTask2 = "Task 2 is running\r\n";

int main(void)
{
    xTaskCreate(
        vTaskFunction,
        "Task 1",
        1000,
        (void*)pcTextForTask1,
        1,
        NULL);

    xTaskCreate(
        vTaskFunction,
        "Task 2",
        1000,
        (void*)pcTextForTask2,
        1,
        NULL);

    vTaskStartScheduler();

    for (;;);

    return 0;
}

void vTaskFunction(void* pvParameters)
{
    char* pcTaskName;
    volatile uint32_t ul;

    pcTaskName = (char*)pvParameters;

    for (;;) {
        vPrintString(pcTaskName);

        for (ul = 0; ul < mainDELAY_LOOP_COUNT; ul++) {

        }
    }
}

