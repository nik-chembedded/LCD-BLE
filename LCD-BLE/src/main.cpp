#include <Arduino.h>
#include "gen_Defines.h"
#include "gen_Tasks.h"

void setup() {
  /* Init FreeRTOS Tasks and MSG Qeue*/
#ifdef DEBUG_MODE
  SERIAL_DEBUG_INIT(9600);
#endif
  /* Initialize freeRTOS and start all tasks */
  fRtosInit();
}

void loop() {
  /* Empty. Things are done in Tasks. */
}