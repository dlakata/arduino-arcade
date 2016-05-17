#include "Arcade.h"

#define CANDY_SIZE 40
#define NUM_SQUARES 8

TouchScreen *cc_ts;
Adafruit_TFTLCD *cc_tft;

#define NUM_COLORS 7
int colors[NUM_COLORS] = {
  BLUE, RED, GREEN, CYAN, MAGENTA, YELLOW, WHITE
};

CandyCrush::CandyCrush(TouchScreen *_ts, Adafruit_TFTLCD *_tft) : 
Game(_ts, _tft) {
  cc_ts = _ts;
  cc_tft = _tft;
}

void draw_candy(int x, int y, int color) {
  cc_tft->fillRect(x * CANDY_SIZE, y * CANDY_SIZE, CANDY_SIZE, CANDY_SIZE, BLACK);
  cc_tft->drawRect(x * CANDY_SIZE, y * CANDY_SIZE, CANDY_SIZE, CANDY_SIZE, WHITE);
  cc_tft->fillCircle(x * CANDY_SIZE + CANDY_SIZE / 2, y * CANDY_SIZE + CANDY_SIZE / 2, CANDY_SIZE / 2 - 5, color);
}

void CandyCrush::reset() {
  int color;
  for (int i = 0; i < NUM_SQUARES; i++) {
    for (int j = 0; j < NUM_SQUARES; j++) {
      color = random(0, NUM_COLORS);
      draw_candy(j, i, colors[color]);
    }
  }
}

void CandyCrush::run() {

}

