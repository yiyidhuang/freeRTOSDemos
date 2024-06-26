﻿
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "supporting_functions.h"

static void vSenderTask(void* pvParameters);
static void vReceiverTask(void* pvParameters);

QueueHandle_t xQueue;

int main(void)
{
    xQueue = xQueueCreate(5, sizeof(int32_t));

    if (xQueue != NULL) {
        xTaskCreate(vSenderTask, "Sender1", 500, (void*)100, 1, NULL);
        xTaskCreate(vSenderTask, "Sender2", 500, (void*)200, 1, NULL);

        xTaskCreate(vReceiverTask, "Receiver", 1000, NULL, 2, NULL);

        vTaskStartScheduler();
    }

    for (;;);

    return 0;
}

static void vSenderTask(void* pvParameters)
{
    int32_t lValueToSend;
    BaseType_t xStatus;

    lValueToSend = (int32_t)pvParameters;

    for (;;) {
        xStatus = xQueueSendToBack(xQueue, &lValueToSend, 0);

        if (xStatus != pdPASS) {
            vPrintString("Could not send to the queue.\r\n");
        }
    }
}

static void vReceiverTask(void* pvParameters)
{
    int32_t lReceivedValue = 0;
    BaseType_t xStatus;
    const TickType_t xTicksToWait = pdMS_TO_TICKS(100UL);

    for (;;) {
        if (uxQueueMessagesWaiting(xQueue) != 0) {
            vPrintString("Queue should have been empty!\r\n");
        }

        xStatus = xQueueReceive(xQueue, &lReceivedValue, xTicksToWait);

        if (xStatus == pdPASS) {
            vPrintStringAndNumber("Received = ", lReceivedValue);
        }
        else {
            vPrintString("Could not receive from the queue.\r\n");
        }
    }
}
