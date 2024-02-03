#include <Arduino.h>
#include "gen_Defines.h"
#include "gen_Tasks.h"
#include "lcd_Manager.h"
#include "bt_Manager.h"

// Declare a mutex Semaphore Handle which we will use to manage the Serial Port.
// It will be used to ensure only one Task is accessing this resource at any time.
SemaphoreHandle_t xSerialSemaphore;

// Declare a message queue for inter task communication
QueueHandle_t msgQeue; 

// define two Tasks for DigitalWrite & AnalogRead
void TaskBlManager(void *pvParameters);
void TaskLcdManager(void *pvParameters);

static rc_status fRtos_serialSemaphoreInit() {
    // Semaphores are useful to stop a Task proceeding, where it should be paused to wait,
    // because it is sharing a resource, such as the Serial port.
    // Semaphores should only be used whilst the scheduler is running, but we can set it up here.
    if ( xSerialSemaphore == NULL )  // Check to confirm that the Serial Semaphore has not already been created.
    {
      xSerialSemaphore = xSemaphoreCreateMutex();  // Create a mutex semaphore we will use to manage the Serial Port
      if ( ( xSerialSemaphore ) != NULL )
        xSemaphoreGive( ( xSerialSemaphore ) );  // Make the Serial Port available for use, by "Giving" the Semaphore.
    }
    return RC_OK;
}


static rc_status fRtos_QeuesInit() {
    // Set up msg queue.
    msgQeue = xQueueCreate(10, sizeof(frtos_Message_t));
    if (!msgQeue) {
        return RC_INIT_ERROR;
    }
    return RC_OK;
}

static rc_status fRtos_TasksInit() {
    xTaskCreate(
      TaskBlManager
      ,  "BluetoohManager"  // A name just for humans
      ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
      ,  NULL //Parameters for the task
      ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
      ,  NULL ); //Task Handle  
    xTaskCreate(
      TaskLcdManager
      ,  "LCDScreenManager" // A name just for humans
      ,  128  // Stack size
      ,  NULL //Parameters for the task
      ,  1  // Priority
      ,  NULL ); //Task Handle 

    return RC_OK;
}

rc_status fRtosInit() {
    // Init USB serial port semaphores
    fRtos_serialSemaphoreInit();
    // Init inter-task message qeue
    fRtos_QeuesInit();
    // Now set up two Tasks to run independently.
    fRtos_TasksInit();
    // Now the Task scheduler, which takes over control of scheduling individual Tasks, is automatically started.
    return RC_OK;
}

rc_status fRtosSendMsg(frtos_TaskId_e taskId, 
                       uint8_t subTaksId, 
                       int val, 
                       TickType_t tick) {
    frtos_Message_t qSndMsg;

    RTOS_SEND(msgQeue, qSndMsg, taskId, subTaksId, val, tick);
    return RC_OK;
}

static rc_status queuePeek(frtos_TaskId_e id) {
    rc_status rc = RC_RTOS_MSG_NO_MATCH;
    frtos_Message_t tmpQueueMsg;

    if ((msgQeue != 0) && (uxQueueMessagesWaiting(msgQeue))) {
        /* Peek a message on the created queue.  Block for 5 ticks if a
           message is not immediately available. 
        */
        if (xQueuePeek(msgQeue, &tmpQueueMsg, (TickType_t) 5)) {
            // pcRxedMessage now points to the struct AMessage variable posted
            // by vATask, but the item still remains on the queue.
            if(tmpQueueMsg.taskId == id) {
              QUEUE_PEEK_MATCH_LOG(tmpQueueMsg.taskId);
              return RC_OK;
            }
        }
    }

    return rc;
}

static rc_status peekAndRcvMsg(frtos_Message_t* qRcvMsg, frtos_TaskId_e id) {
    rc_status rc = RC_RTOS_MSG_NO_MATCH;

    if (queuePeek(id) == RC_OK) {
      if (uxQueueMessagesWaiting(msgQeue)) {
          xQueueReceive(msgQeue, qRcvMsg, (TickType_t) 5);
          rc = RC_OK;
      }
    }
    return rc;
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

void TaskBlManager( void *pvParameters __attribute__((unused)) )  // This is a Task.
{
    frtos_Message_t qRcvMsg;
    //frtos_Message_t qSndMsg;

    /* Calling BT Init on task start */
    btInit();
    for (;;) // A Task shall never return or exit.
    {
      if (peekAndRcvMsg(&qRcvMsg, RTOS_TASK_BT) == RC_OK) {
        processBtMsg(&qRcvMsg);
      }
      // See if we can obtain or "Take" the Serial Semaphore.
      // If the semaphore is not available, wait 5 ticks of the Scheduler to see if it becomes free.
      if ( xSemaphoreTake(xSerialSemaphore, (TickType_t) 5) == pdTRUE)
      {
        // We were able to obtain or "Take" the semaphore and can now access the shared resource.
        // We want to have the Serial Port for us alone, as it takes some time to print,
        // so we don't want it getting stolen during the middle of a conversion.
        // print out the state of the button:

        xSemaphoreGive(xSerialSemaphore); // Now free or "Give" the Serial Port for others.
      }
      //RTOS_SEND(msgQeue, qSndMsg, RTOS_TASK_LCD, LCD_TASK_THREE, 456, TICK * 5);
      //xQueueSendToBack(msgQeue, &qSndMsg, TICK * 10);
      //
      //fRtosSendMsg(RTOS_TASK_LCD, LCD_TASK_ONE, 456, TICK * 5);
      //vTaskDelay(ONE_SEC * 2);  // One Second Delay (1000ms ~ 15ms * 67 Ticks)
      vTaskDelay(TICK * 5);  // 75ms = 15ms * 5 Ticks - Delay
    }
}

void TaskLcdManager( void *pvParameters __attribute__((unused)) )  // This is a Task.
{
    frtos_Message_t qRcvMsg;
    //frtos_Message_t qSndMsg;

    /* Calling LCD Init on task start */
    lcdInit();
    for (;;) // A Task shall never return or exit.
    {
      if (peekAndRcvMsg(&qRcvMsg, RTOS_TASK_LCD) == RC_OK) {
        processLcdMsg(&qRcvMsg);
      }
      // See if we can obtain or "Take" the Serial Semaphore.
      // If the semaphore is not available, wait 5 ticks of the Scheduler to see if it becomes free.
      if (xSemaphoreTake(xSerialSemaphore, (TickType_t) 5) == pdTRUE)
      {
        // We were able to obtain or "Take" the semaphore and can now access the shared resource.
        // We want to have the Serial Port for us alone, as it takes some time to print,
        // so we don't want it getting stolen during the middle of a conversion.
        // print out the value you read:
 
        xSemaphoreGive( xSerialSemaphore ); // Now free or "Give" the Serial Port for others.
      }
      //xQueueSendToBack(msgQeue, &qSndMsg, TICK * 10);
      //RTOS_SEND(msgQeue, qSndMsg, RTOS_TASK_BT, BT_TASK_TWO, 123, TICK * 5);
      //fRtosSendMsg(RTOS_TASK_BT, BT_TASK_TWO, 123, TICK * 5);
      //vTaskDelay(TICK * 5);  // one tick delay (15ms) in between reads for stability
      vTaskDelay(TICK * 5);  // 75ms = 15ms * 5 Ticks - Delay
    }
}