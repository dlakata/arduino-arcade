
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include <TouchScreen.h>

#include "Arcade.h"

#define NUM_GAMES 2
#define LINE_HEIGHT 100
#define LINE_OFFSET 100

TouchScreen arcade_ts = TouchScreen(XP, YP, XM, YM, 300);
Adafruit_TFTLCD arcade_tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

Bingo bingo(&arcade_ts, &arcade_tft);
CandyCrush candyCrush(&arcade_ts, &arcade_tft);

char* names[NUM_GAMES] = {
  "BINGO",
  "CANDY CRUSH"
};

void setup(void) {
  Serial.begin(9600);

  arcade_tft.reset();

  uint16_t identifier = arcade_tft.readID();
  arcade_tft.begin(identifier);

  arcade_tft.fillScreen(BLACK);
  arcade_tft.setCursor(0, 0);
  arcade_tft.setTextColor(WHITE);
  arcade_tft.setTextSize(5);
  arcade_tft.println("The Arcade");
  arcade_tft.setTextSize(2);
  for (int i = 0; i < NUM_GAMES; i++) {
    arcade_tft.setCursor(LINE_OFFSET, LINE_HEIGHT + LINE_HEIGHT * i);
    arcade_tft.println(names[i]);
  }
  pinMode(13, OUTPUT);
}

void loop()
{
  TSPoint p = arcade_ts.getPoint();
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  int pressed = p.z > MINPRESSURE && p.z < MAXPRESSURE;
  if (pressed) {
    p.x = map(p.x, TS_MINX, TS_MAXX, arcade_tft.width(), 0);
    p.y = map(p.y, TS_MINY, TS_MAXY, arcade_tft.height(), 0);
    if (LINE_HEIGHT < p.y && p.y < LINE_HEIGHT * 2) {
      bingo.reset();
      while (1) {
        bingo.run();
      }
    } else if (LINE_HEIGHT * 2 < p.y && p.y < LINE_HEIGHT * 3) {
      candyCrush.reset();
      while (1) {
        candyCrush.run();
      }
    }
  }
}


