#ifndef GEN_LCD_CHARACTER_H
#define GEN_LCD_CHARACTER_H
#include <Arduino.h>

#define BLANK 0
#define FILL  1

byte blank[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

byte fill[8] = {
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111
};

#endif /* GEN_LCD_CHARACTER_H */