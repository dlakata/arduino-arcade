#include "Arcade.h"

#define CANDY_SIZE 40
#define NUM_CANDIES 8

#define NUM_COLORS 7
int colors[NUM_COLORS] = {
  BLUE, RED, GREEN, CYAN, MAGENTA, YELLOW, WHITE
};

CandyCrush::CandyCrush(TouchScreen *_ts, Adafruit_TFTLCD *_tft) {
  this->ts = _ts;
  this->tft = _tft;
}

void CandyCrush::draw_candy(int x, int y, int color) {
  tft->fillRect(x * CANDY_SIZE, y * CANDY_SIZE, CANDY_SIZE, CANDY_SIZE, BLACK);
  tft->drawRect(x * CANDY_SIZE, y * CANDY_SIZE, CANDY_SIZE, CANDY_SIZE, WHITE);
  tft->fillCircle(x * CANDY_SIZE + CANDY_SIZE / 2, y * CANDY_SIZE + CANDY_SIZE / 2, CANDY_SIZE / 2 - 5, color);
}

void CandyCrush::reset() {
  int color;
  for (int i = 0; i < NUM_CANDIES; i++) {
    for (int j = 0; j < NUM_CANDIES; j++) {
      color = random(0, NUM_COLORS);
      draw_candy(j, i, colors[color]);
    }
  }
}

void CandyCrush::run() {

}
