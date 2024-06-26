

#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include "timers.h"

#include "supporting_functions.h"

#define mainINTERRUPT_NUMBER        3

#define mainFIRST_TASK_BIT  (1UL << 0UL)
#define mainSECOND_TASK_BIT  (1UL << 1UL)
#define mainISR_BIT  (1UL << 2UL)

static void vIntegerGenerator(void* pvParameters);
static void vEventBitSettingTask(void* pvParameters);
static void vEventBitReadingTask(void* pvParameters);

void vPrintStringFromDaemonTask(void* pvParameter1, uint32_t ulParameter2);

static uint32_t ulEventBitSettingISR(void);

EventGroupHandle_t xEventGroup;

int main(void)
{
    xEventGroup = xEventGroupCreate();

    xTaskCreate(vEventBitSettingTask, "BitSetter", 1000, NULL, 1, NULL);

    xTaskCreate(vEventBitReadingTask, "BitReader", 1000, NULL, 2, NULL);

    xTaskCreate(vIntegerGenerator, "IntGen", 1000, NULL, 3, NULL);

    vPortSetInterruptHandler(mainINTERRUPT_NUMBER, ulEventBitSettingISR);

    vTaskStartScheduler();

    for (;;);

    return 0;
}

static void vEventBitSettingTask(void* pvParameters)
{
    const TickType_t xDelay200ms = pdMS_TO_TICKS(200UL), xDontBlock = 0;

    for (;;) {
        vTaskDelay(xDelay200ms);

        vPrintString("Bit setting task -\t about to set bit 0.\r\n");
        xEventGroupSetBits(xEventGroup, mainFIRST_TASK_BIT);
        vTaskDelay(xDelay200ms);

        vPrintString("Bit setting task -\t about to set bit 1.\r\n");
        xEventGroupSetBits(xEventGroup, mainSECOND_TASK_BIT);
    }
}

static uint32_t ulEventBitSettingISR(void)
{
    BaseType_t xHigherPriorityTaskWoken;

    static const char* pcString = "Bit setting ISR -\t about to set bit 2.\r\n";

    xHigherPriorityTaskWoken = pdFALSE;

    xTimerPendFunctionCallFromISR(vPrintStringFromDaemonTask, (void*)pcString, 0, &xHigherPriorityTaskWoken);

    xEventGroupSetBitsFromISR(xEventGroup, mainISR_BIT, &xHigherPriorityTaskWoken);

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

static void vEventBitReadingTask(void* pvParameters)
{
    const EventBits_t xBitsToWaitFor = (mainFIRST_TASK_BIT | mainSECOND_TASK_BIT | mainISR_BIT);
    EventBits_t xEventGroupValue;

    for (;;) {
        xEventGroupValue = xEventGroupWaitBits(
            xEventGroup,
            xBitsToWaitFor,
            pdTRUE,
            pdFALSE,
            portMAX_DELAY);

        if ((xEventGroupValue & mainFIRST_TASK_BIT) != 0) {
            vPrintString("Bit reading task -\t event bit 0 was set\r\n");
        }

        if ((xEventGroupValue & mainSECOND_TASK_BIT) != 0) {
            vPrintString("Bit reading task -\t event bit 1 was set\r\n");
        }

        if ((xEventGroupValue & mainISR_BIT) != 0) {
            vPrintString("Bit reading task -\t event bit 2 was set\r\n");
        }

        vPrintString("\r\n");
    }
}

void vPrintStringFromDaemonTask(void* pvParameter1, uint32_t ulParameter2)
{
    vPrintString((const char*)pvParameter1);
}

static void vIntegerGenerator(void* pvParameters)
{
    TickType_t xLastExecutionTime;
    const TickType_t xDelay500ms = pdMS_TO_TICKS(500UL);

    xLastExecutionTime = xTaskGetTickCount();

    for (;;) {
        vTaskDelayUntil(&xLastExecutionTime, xDelay500ms);

        vPortGenerateSimulatedInterrupt(mainINTERRUPT_NUMBER);
    }
}

