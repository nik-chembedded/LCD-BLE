#ifndef GEN_LCD_MANAGER_H
#define GEN_LCD_MANAGER_H

#define LCD_ROWS    2
#define LCD_COLS    16

typedef rc_status (*lcdSubTask_f) (frtos_Message_t* qMsg);

void processLcdMsg(frtos_Message_t* qMsg);
void lcdInit();

#endif /* GEN_LCD_MANAGER_H */