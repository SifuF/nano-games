#include <LiquidCrystal.h>
#include "Snake.h"
#include "Snow.h"

#define GAME_SNAKE 0
#define GAME_SNOW 1

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

bool menu = true;
int game = GAME_SNAKE;

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
Snow snow;

void setup() {
  screen[width * height] = '\0';
  lcd.begin(20, 4);
  pinMode(buttonLeft, INPUT);
  pinMode(buttonRight, INPUT);
}

void loop() {
  clearScreen();
  if(menu) {
    const char* text = "Choose game...";
    const char* game1 = "1. Snake";
    const char* game2 = "2. Snow";

    int i = 0;
    while (text[i] != '\0') {
      writeChar(text[i], 3 + i, 0);
      ++i;
    }

    i = 0;
    while (game1[i] != '\0') {
      writeChar(game1[i], i, 1);
      ++i;
    }
    
    i = 0;
    while (game2[i] != '\0') {
      writeChar(game2[i], i, 2);
      ++i;
    }

    if(digitalRead(buttonLeft)) {
      game = GAME_SNAKE;
      menu = false;
    }
    if(digitalRead(buttonRight)) {
      game = GAME_SNOW;
      menu = false;
    }
  }
  else {
    if(game == GAME_SNAKE) {
      snake.drawFrame();
    }
    else if(game == GAME_SNOW) {
      snow.drawFrame();
    }
  }
  lcd.print(screen);
  delay(200);
}
