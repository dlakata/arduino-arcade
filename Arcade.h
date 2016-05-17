#ifndef Arcade_h
#define Arcade_h

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include <TouchScreen.h>

class Bingo
{
public:
  Bingo(TouchScreen *_ts, Adafruit_TFTLCD *_tft);
  void reset();
  void run();
private:
  void draw_square(int x, int y, int color);
  void write_square(int x, int y, int color);
  void draw_bingos_left();
  int victory(int x, int y);
  TouchScreen *ts;
  Adafruit_TFTLCD *tft;
};

class CandyCrush
{
public:
  CandyCrush(TouchScreen *_ts, Adafruit_TFTLCD *_tft);
  void reset();
  void run();
private:
  void draw_candy(int x, int y, int color);
  TouchScreen *ts;
  Adafruit_TFTLCD *tft;
};

#if defined(__SAM3X8E__)
#undef __FlashStringHelper::F(string_literal)
#define F(string_literal) string_literal
#endif

#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin

#define TS_MINX 150
#define TS_MINY 120
#define TS_MAXX 920
#define TS_MAXY 940

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
#define LCD_RESET A4

#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

#define MINPRESSURE 800
#define MAXPRESSURE 1000

#endif
