
#include "FreeRTOS.h"
#include "task.h"

#include "supporting_functions.h"

#define mainDELAY_LOOP_COUNT        (0xffffff)

void vTask1(void* pvParameters);
void vTask2(void* pvParameters);

int main(void)
{
    xTaskCreate(vTask1,
        "Task 1",
        1000,
        NULL,
        1,
        NULL);

    xTaskCreate(vTask2, "Task 2", 1000, NULL, 1, NULL);

    vTaskStartScheduler();

    for (;;);

    return 0;
}

void vTask1(void* pvParameters)
{
    const char* pcTaskName = "Task 1 is running\r\n";

    volatile uint32_t ul;

    for (;;) {
        vPrintString(pcTaskName);

        for (ul = 0; ul < mainDELAY_LOOP_COUNT; ul++) {

        }
    }
}

void vTask2(void* pvParameters)
{
    const char* pcTaskName = "Task 2 is running\r\n";
    volatile uint32_t ul;

    for (;;) {
        vPrintString(pcTaskName);

        for (ul = 0; ul < mainDELAY_LOOP_COUNT; ul++) {

        }
    }
}
