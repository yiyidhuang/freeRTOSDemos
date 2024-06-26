
#include "FreeRTOS.h"
#include "task.h"

#include "supporting_functions.h"

void vTask1(void* pvParameters);
void vTask2(void* pvParameters);

TaskHandle_t xTask2Handle;

int main(void)
{
    xTaskCreate(vTask1, "Task 1", 1000, NULL, 2, NULL);

    xTaskCreate(vTask2, "Task 2", 1000, NULL, 1, &xTask2Handle);

    vTaskStartScheduler();

    for (;;);

    return 0;
}

void vTask1(void* pvParameters)
{
    UBaseType_t uxPriority;

    uxPriority = uxTaskPriorityGet(NULL);

    for (;;) {
        vPrintString("Task 1 is running\r\n");

        vPrintString("About to raise the Task 2 priority\r\n");

        vTaskPrioritySet(xTask2Handle, (uxPriority + 1));
    }
}

void vTask2(void* pvParameters)
{
    UBaseType_t uxPriority;

    uxPriority = uxTaskPriorityGet(NULL);

    for (;;) {
        vPrintString("Task 2 is running\r\n");

        vPrintString("About to lower the Task2 priority\r\n");

        vTaskPrioritySet(NULL, (uxPriority - 2));
    }
}