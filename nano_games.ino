#include <LiquidCrystal.h>
#include "Snake.h"

const int rs = 9;
const int rw = 10;
const int enable = 11;
const int d4 = 4;
const int d5 = 5;
const int d6 = 6;
const int d7 = 7;
LiquidCrystal lcd(rs, rw, enable, d4, d5, d6, d7);

const uint8_t width = 20;
const uint8_t height = 4;
char screen[width * height + 1];

const int buttonLeft = 2;
const int buttonRight = 3;

void clearScreen() {
  lcd.clear();
  memset(screen, ' ', width * height);
}

void writeChar(char c, int x, int y) {
  if (x < 0 || x >= width) {
    screen[0] = 'E';
    return;
  }
  switch (y) {
    case 0:
      screen[x] = c;
      break;
    case 1:
      screen[x + width * 2] = c;
      break;
    case 2:
      screen[x + width * 1] = c;
      break;
    case 3:
      screen[x + width * 3] = c;
      break;
    default:
      screen[0] = 'E';
  }
}

Snake snake;

void setup() {
  screen[width * height] = '\0';
  lcd.begin(20, 4);
  pinMode(buttonLeft, INPUT);
  pinMode(buttonRight, INPUT);
}

void loop() {
  clearScreen();
  snake.drawFrame();
  lcd.print(screen);
  delay(200);
}
