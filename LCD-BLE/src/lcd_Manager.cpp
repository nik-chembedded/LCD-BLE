#include <Arduino.h>
#include <LiquidCrystal.h>
#include "gen_Defines.h"
#include "gen_Tasks.h"
#include "lcd_Manager.h"
#include "lcd_Characters.h"

LiquidCrystal lcd(LCD_RS, 
                  LCD_EN, 
                  LCD_D4, 
                  LCD_D5, 
                  LCD_D6, 
                  LCD_D7);

static rc_status lcd_Test_One(frtos_Message_t* qMsg __attribute__((unused))) {
  uint8_t i;
  lcd.setCursor(0,0);
  for(i = 0 ; i <= LCD_COLS; i++) {
    lcd.write(byte(BLANK));
    lcd.setCursor(i,0);
  }
  return RC_OK;
}

static rc_status lcd_Test_Two(frtos_Message_t* qMsg __attribute__((unused))) {
  uint8_t i;
  lcd.setCursor(0,0);
  for(i = 0 ; i <= LCD_COLS; i++) {
    lcd.write(byte(FILL));
    lcd.setCursor(i,0);
  }
  return RC_OK;
}

static rc_status lcd_Test_Three(frtos_Message_t* qMsg __attribute__((unused))) {
  uint8_t i;
  lcd.setCursor(0,0);
  for(i = 0 ; i <= LCD_COLS/2; i++) {
    lcd.write(byte(FILL));
    lcd.setCursor(i,1);
  }
  return RC_OK;
}

lcdSubTask_f lcdSubTaskArr[LCD_SUBTUSK_MAX] = {
    lcd_Test_One,
    lcd_Test_Two,
    lcd_Test_Three
};

void lcdInit() {
  /* Set up the LCD's number of columns and rows */
  lcd.begin(LCD_COLS, LCD_ROWS);
  /* Print a message to the LCD. */
  //lcd.print("My LCD Hello!");
  lcd.createChar(BLANK, blank);
  lcd.createChar(FILL, fill);
  lcd.clear();
}

void processLcdMsg(frtos_Message_t* qMsg) {
    frtos_LcdSubTaskId_e subTask = (frtos_LcdSubTaskId_e)qMsg->subTaskId;
    lcdSubTaskArr[subTask](qMsg);
}

