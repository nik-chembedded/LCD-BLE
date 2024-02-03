#include <Arduino.h>
#include "gen_Defines.h"
#include "gen_Tasks.h"
#include "bt_Manager.h"

void btInit() {

}

void processBtMsg(frtos_Message_t* qMsg) {
    frtos_BtSubTaskId_e subTask = (frtos_BtSubTaskId_e)qMsg->subTaskId;

    switch(subTask) {
      case BT_TASK_ONE:
        break;
      case BT_TASK_TWO:
        break;
      case BT_TASK_THREE:
        break;     
      default:
        break;
    }
}


