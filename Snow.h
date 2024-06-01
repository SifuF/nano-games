void clearScreen();
void writeChar(char c, int x, int y);
extern const uint8_t width;
extern const uint8_t height;
extern const int buttonLeft;
extern const int buttonRight;
extern bool menu;

struct Snow {
  char c = 33; //33 to 127;

  void drawFrame() {
    int particles = rand() % (width * height / 3);
    for (int i = 0; i < particles; ++i) {
      int x = rand() % width;
      int y = rand() % height;
      writeChar(c, x, y);
    }

    if(digitalRead(buttonRight)) {
        c = 33 + (rand() % (128-33));
    }

    if(digitalRead(buttonLeft)) {
        menu = true;
    }
  }
};
