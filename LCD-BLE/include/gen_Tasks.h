#ifndef GEN_TASKS_H
#define GEN_TASKS_H

#include <Arduino_FreeRTOS.h>
#include <semphr.h>  // add the FreeRTOS functions for Semaphores (or Flags).

#define TICK        (1)             // Tick = 15ms
#define ONE_SEC     (TICK * 67)     // One Second Delay (1000ms ~ 15ms * 67)

typedef enum frtos_TaskId {
    RTOS_TASK_NONE,
    RTOS_TASK_BT,
    RTOS_TASK_LCD,
    RTOS_TASK_MAX
} frtos_TaskId_e;

typedef enum frtos_LcdSubTaskId {
    LCD_TASK_ONE,
    LCD_TASK_TWO,
    LCD_TASK_THREE,
    LCD_SUBTUSK_MAX
} frtos_LcdSubTaskId_e;

typedef enum frtos_BtSubTaskId {
    BT_TASK_ONE,
    BT_TASK_TWO,
    BT_TASK_THREE,
    BT_SUBTUSK_MAX
} frtos_BtSubTaskId_e;

typedef struct frtos_Message {
    frtos_TaskId_e      taskId;
    uint8_t             subTaskId;
    int                 val;
} frtos_Message_t;

rc_status fRtosSendMsg(frtos_TaskId_e taskId, uint8_t subTaksId, int val, TickType_t tick);
rc_status fRtosInit();

#endif /* GEN_TASKS_H */