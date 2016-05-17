#ifndef Game_h
#define Game_h

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include <TouchScreen.h>

class Bingo
{
public:
  Bingo(TouchScreen *_ts, Adafruit_TFTLCD *_tft);
  void reset();
  void run();
};

#endif

