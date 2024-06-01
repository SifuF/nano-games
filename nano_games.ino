#include <LiquidCrystal.h>

#define DIR_UP 0
#define DIR_DOWN 1
#define DIR_LEFT 2
#define DIR_RIGHT 4

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
const int MAX_LENGTH = 15;

bool running = true;

const int buttonLeft = 2;
const int buttonRight = 3;

struct Player {
  char c = 'O';
  int x = 0;
  int y = 1;
  int direction = DIR_RIGHT;
};

int length = 1;
Player player[MAX_LENGTH];

char food = 'X';
int foodX = 0;
int foodY = 0;

void clearScreen() {
  memset(screen, ' ', width * height);
}

void clearLcd() {
}

void setFood() {
  foodX = rand() % width;
  foodY = rand() % height;
}

void setup() {
  screen[width * height] = '\0';
  lcd.begin(20, 4);
  pinMode(buttonLeft, INPUT);
  pinMode(buttonRight, INPUT);
  reset();
}

void reset() {
  clearScreen();
  setFood();
  length = 1;
  running = true;
  player[0].x = 0;
  player[0].y = 1;
  player[0].direction = DIR_RIGHT;
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

void boundsCheck(int& posX, int& posY) {
  if (posX >= width) {
    posX = 0;
  }
  if (posX < 0) {
    posX = width - 1;
  }
  if (posY >= height) {
    posY = 0;
  }
  if (posY < 0) {
    posY = height - 1;
  }
}

void checkDeath() {
  for (int i = 1; i < length; ++i) {
    if (player[0].x == player[i].x && player[0].y == player[i].y) {
      running = false;
    }
  }
}

void buildGameOver() {
  const char* text1 = "GAME OVER!";
  const char* text2 = "You scored";

  int i = 0;
  while (text1[i] != '\0') {
    writeChar(text1[i], 5 + i, 1);
    ++i;
  }

  i = 0;
  while (text2[i] != '\0') {
    writeChar(text2[i], 4 + i, 2);
    ++i;
  }

  writeChar(length + '0', width - 5, 2);
}

void loop() {
  lcd.clear();
  clearLcd();
  clearScreen();
  if (running) {
    if (player[0].x == foodX && player[0].y == foodY) {
      setFood();
      if (length < MAX_LENGTH) {
        length++;
      }
    }

    checkDeath();

    for (int i = 0; i < length - 1; ++i) {
      player[length - 1 - i].x = player[length - 2 - i].x;
      player[length - 1 - i].y = player[length - 2 - i].y;
    }

    switch (player[0].direction) {
      case DIR_UP:
        player[0].y--;
        break;
      case DIR_DOWN:
        player[0].y++;
        break;
      case DIR_LEFT:
        player[0].x--;
        break;
      default:
        player[0].x++;
        break;
    }

    if (digitalRead(buttonRight)) {
      if (player[0].direction == DIR_LEFT || player[0].direction == DIR_RIGHT) {
        player[0].direction = DIR_UP;
      } else if (player[0].direction == DIR_UP || player[0].direction == DIR_DOWN) {
        player[0].direction = DIR_RIGHT;
      }
    }

    if (digitalRead(buttonLeft)) {
      if (player[0].direction == DIR_LEFT || player[0].direction == DIR_RIGHT) {
        player[0].direction = DIR_DOWN;
      } else if (player[0].direction == DIR_UP || player[0].direction == DIR_DOWN) {
        player[0].direction = DIR_LEFT;
      }
    }

    boundsCheck(player[0].x, player[0].y);

    for (int i = 0; i < length; ++i) {
      writeChar(player[i].c, player[i].x, player[i].y);
    }

    writeChar(food, foodX, foodY);
  } 
  else {
    buildGameOver();
    if (digitalRead(buttonLeft) || digitalRead(buttonRight)) {
      reset();
    }
  }

  lcd.print(screen);
  const int delayT = 200 - (length - 1) * 10;
  if (delayT > 0) {
    delay(delayT);
  }
}
