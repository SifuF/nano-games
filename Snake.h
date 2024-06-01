#define DIR_UP 0
#define DIR_DOWN 1
#define DIR_LEFT 2
#define DIR_RIGHT 4

#define MAX_LENGTH 15

void clearScreen();
void writeChar(char c, int x, int y);
extern const uint8_t width;
extern const uint8_t height;
extern const int buttonLeft;
extern const int buttonRight;
extern bool menu;

struct Snake {
  struct Player {
    char c = 'O';
    int x;
    int y;
    int direction;
  };

  bool running;
  int length;
  Player player[MAX_LENGTH];

  char food = 'X';
  int foodX = 0;
  int foodY = 0;

  Snake() {
    reset();
  }

  void setFood() {
    foodX = rand() % width;
    foodY = rand() % height;
  }

  void reset() {
    setFood();
    length = 1;
    running = true;
    player[0].x = 0;
    player[0].y = 1;
    player[0].direction = DIR_RIGHT;
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
    const char* text3 = "Quit        Continue";

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

    if(length/10) {
      writeChar((length-1)/10 + '0', i + 5, 2);
      writeChar((length-1)%10 + '0', i + 6, 2);
    }
    else {
      writeChar(length - 1 + '0', i + 5, 2);
    }

    i = 0;
    while (text3[i] != '\0') {
      writeChar(text3[i], i, 3);
      ++i;
    }
  }

  void boundsCheck() {
    if (player[0].x >= width) {
      player[0].x = 0;
    }
    if (player[0].x < 0) {
      player[0].x = width - 1;
    }
    if (player[0].y >= height) {
      player[0].y = 0;
    }
    if (player[0].y < 0) {
      player[0].y = height - 1;
    }
  }

  void drawFrame() {
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

      boundsCheck();

      for (int i = 0; i < length; ++i) {
        writeChar(player[i].c, player[i].x, player[i].y);
      }

      writeChar(food, foodX, foodY);
    }
    else {
      buildGameOver();
      if (digitalRead(buttonLeft)) {
        reset();
        menu = true;
      }
      if (digitalRead(buttonRight)) {
        reset();
      }
    }
  }
};

