
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include <TouchScreen.h>

#include "Arcade.h"

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
TouchScreen arcade_ts = TouchScreen(XP, YP, XM, YM, 300);

Adafruit_TFTLCD arcade_tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

Bingo bingo(&arcade_ts, &arcade_tft);

void setup(void) {
  Serial.begin(9600);

  arcade_tft.reset();

  uint16_t identifier = arcade_tft.readID();
  arcade_tft.begin(identifier);
  
  bingo.reset();

  pinMode(13, OUTPUT);
}

void loop()
{
  bingo.run();
}





