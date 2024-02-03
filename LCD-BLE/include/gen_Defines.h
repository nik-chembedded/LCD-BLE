#ifndef GEN_DEFINES_H
#define GEN_DEFINES_H

#define DEBUG

#ifdef DEBUG
#define DEBUG_MODE
#define SERIAL_DEBUG_INIT(baudRate) { \
            Serial.begin(baudRate); \
            while (!Serial) { ; } \
            }
#define SERIAL_PRINTLN(a) Serial.println(a)
#define SERIAL_PRINT(a) Serial.print(a)
#define QUEUE_PEEK_MATCH_LOG(taskId) { \
            switch(taskId) { \
                case RTOS_TASK_BT: SERIAL_PRINTLN("RTOS_TASK_BT MATCH"); \
                    break; \
                case RTOS_TASK_LCD: SERIAL_PRINTLN("RTOS_TASK_LCD MATCH"); \
                    break; \
                default: SERIAL_PRINTLN("BAD TASK ID"); \
                    break; \
            } \
}
#else
#define NON_DEBUG_MODE
#define DO_NONE do { ; } while (0)
#define SERIAL_DEBUG_INIT(baudRate)     DO_NONE
#define SERIAL_PRINTLN(a)               DO_NONE
#define SERIAL_PRINT(a)                 DO_NONE
#define QUEUE_PEEK_MATCH_LOG(taskId)    DO_NONE
#endif

#define RTOS_SEND(queue, item, task, subTask, data, ticksToWait) { \
            item.taskId = task; \
            item.subTaskId = subTask; \
            item.val = data; \
            xQueueSendToBack(queue, &item, ticksToWait); \
}
/* General IO Definitions */
#define BUILTIN_LED 13

/* BT IO Definitions */


/* LCD IO Definitions */
#define LCD_D4 28
#define LCD_D5 30
#define LCD_D6 32
#define LCD_D7 34
#define LCD_RS 29
#define LCD_EN 31
//#define LCD_RW - Grounded Pin

#define rc_status returnCodes_e
typedef enum returnCodes {
    RC_RTOS_MSG_NO_MATCH    =  -4,
    RC_NOT_READY            =  -3,
    RC_INIT_ERROR           =  -2,
    RC_ERROR                =  -1,
    RC_OK                   =   0
} returnCodes_e;

#endif /* GEN_DEFINES_H */