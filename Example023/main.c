
#include <time.h>

#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"

#include "supporting_functions.h"

#define mainFIRST_TASK_BIT      (1UL << 0UL)
#define mainSECOND_TASK_BIT     (1UL << 1UL)
#define mainTHIRD_TASK_BIT      (1UL << 2UL)

static uint32_t prvRand(void);
static void prvSRand(uint32_t ulSeed);

static void vSyncingTask(void* pvParameters);

static uint32_t ulNextRand;

EventGroupHandle_t xEventGroup;

int main(void)
{
    prvSRand((uint32_t)time(NULL));

    xEventGroup = xEventGroupCreate();

    xTaskCreate(vSyncingTask, "Task 1", 1000, (void*)mainFIRST_TASK_BIT, 1, NULL);
    xTaskCreate(vSyncingTask, "Task 2", 1000, (void*)mainSECOND_TASK_BIT, 1, NULL);
    xTaskCreate(vSyncingTask, "Task 3", 1000, (void*)mainTHIRD_TASK_BIT, 1, NULL);

    vTaskStartScheduler();

    for (;;);

    return 0;
}

static void vSyncingTask(void* pvParameters)
{
    const EventBits_t uxAllSyncBits = (mainFIRST_TASK_BIT | mainSECOND_TASK_BIT | mainTHIRD_TASK_BIT);
    const TickType_t xMaxDelay = pdMS_TO_TICKS(4000UL);
    const TickType_t xMinDelay = pdMS_TO_TICKS(200UL);
    TickType_t xDelayTime;
    EventBits_t uxThisTasksSyncBit;

    uxThisTasksSyncBit = (EventBits_t)pvParameters;

    for (;;) {
        xDelayTime = (prvRand() % xMaxDelay) + xMinDelay;

        vTaskDelay(xDelayTime);

        vPrintTwoStrings(pcTaskGetTaskName(NULL), "reached sync point");

        xEventGroupSync(
            xEventGroup,
            uxThisTasksSyncBit,
            uxAllSyncBits,
            portMAX_DELAY);

        vPrintTwoStrings(pcTaskGetTaskName(NULL), "exited sync point");
    }
}

static uint32_t prvRand(void)
{
    const uint32_t ulMultiplier = 0x015a4e35UL, ulIncrement = 1UL;
    uint32_t ulReturn;

    taskENTER_CRITICAL();
    ulNextRand = (ulMultiplier * ulNextRand) + ulIncrement;
    ulReturn = (ulNextRand >> 16UL) & 0x7fffUL;
    taskEXIT_CRITICAL();

    return ulReturn;
}

static void prvSRand(uint32_t ulSeed)
{
    ulNextRand = ulSeed;
}
