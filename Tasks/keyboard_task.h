#ifndef KEYBOARD_TASK_H
#define KEYBOARD_TASK_H

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

void KeyboardTask(void *pvParameters);
void KeyboardTask_Init(void);

#endif /* KEYBOARD_TASK_H */
