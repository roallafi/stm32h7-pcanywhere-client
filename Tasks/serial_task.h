#ifndef SERIAL_TASK_H
#define SERIAL_TASK_H

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "protocol.h"

extern QueueHandle_t screen_queue;
extern QueueHandle_t keyboard_queue;

void SerialTask(void *pvParameters);
int SerialTask_SendHandshake(void);
int SerialTask_WaitForHandshake(void);

#endif /* SERIAL_TASK_H */
