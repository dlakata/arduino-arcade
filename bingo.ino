#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include <TouchScreen.h>

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

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
// optional
#define LCD_RESET A4

// Assign human-readable names to some common 16-bit color values:
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

#define BOXSIZE 64
#define BINGO_OFFSET 32
#define PENRADIUS 3

#define NUM_SQUARES 5
#define MIN_NUM 10
#define MAX_NUM 99

int numbers[NUM_SQUARES][NUM_SQUARES];
char BINGO[] = "BINGO";

int rand_x, rand_y;

void draw_square(int x, int y, int color) {
  tft.fillRect(x * BOXSIZE, BINGO_OFFSET + y * BOXSIZE, BOXSIZE, BOXSIZE, color);
  tft.drawRect(x * BOXSIZE, BINGO_OFFSET + y * BOXSIZE, BOXSIZE, BOXSIZE, WHITE);
}

void write_square(int x, int y, int num) {
  tft.setCursor(x * BOXSIZE + BOXSIZE / 2 - 8, BINGO_OFFSET + y * BOXSIZE + BOXSIZE / 2 - 8);
  tft.println(num);
}

int get_random_number() {
  return random(MIN_NUM, MAX_NUM);
}

void get_random_square() {
  rand_x = random(0, NUM_SQUARES);
  rand_y = random(0, NUM_SQUARES);
  while (numbers[rand_x][rand_y] == -1) {
    rand_x = random(0, NUM_SQUARES);
    rand_y = random(0, NUM_SQUARES);
  }
}

void reset_board() {
  tft.fillScreen(BLACK);

  randomSeed(analogRead(0));

  for (int i = 0; i < NUM_SQUARES; i++) {
    for (int j = 0; j < NUM_SQUARES; j++) {
      draw_square(j, i, BLUE);
    }
  }
  tft.setTextColor(WHITE);
  tft.setTextSize(2);

  for (int i = 0; i < NUM_SQUARES; i++) {
    tft.setCursor(i * BOXSIZE + BOXSIZE / 2 - 8, 8);
    tft.println(BINGO[i]);
  }
  long num = get_random_number();
  for (int i = 0; i < NUM_SQUARES; i++) {
    for (int j = 0; j < NUM_SQUARES; j++) {
      numbers[i][j] = num;
      write_square(i, j, num);
      num = get_random_number();
    }
  }
  get_random_square();
}

void setup(void) {
  Serial.begin(9600);

  tft.reset();

  uint16_t identifier = tft.readID();
  tft.begin(identifier);

  reset_board();

  pinMode(13, OUTPUT);
}

#define MINPRESSURE 100
#define MAXPRESSURE 1000

void determine_square(TSPoint p, int *box_x, int *box_y) {
  *box_x = p.x / BOXSIZE;
  *box_y = (p.y - BINGO_OFFSET) / BOXSIZE;
  if (*box_y > NUM_SQUARES - 1 || *box_x > NUM_SQUARES - 1) {
    *box_x = -1;
    *box_y = -1;
    return;
  }
}

void loop()
{
  digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);

  // if sharing pins, you'll need to fix the directions of the touchscreen pins
  //pinMode(XP, OUTPUT);
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  //pinMode(YM, OUTPUT);

  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
    // scale from 0->1023 to tft.width
    p.x = map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
    p.y = map(p.y, TS_MINY, TS_MAXY, tft.height(), 0);

    int x, y;
    determine_square(p, &x, &y);
    if (x == rand_x && y == rand_y) {
      numbers[rand_x][rand_y] = -1;
      draw_square(x, y, RED);
      get_random_square();
    }
    tft.fillRect(1 * BOXSIZE, 6 * BOXSIZE, BOXSIZE, BOXSIZE, BLACK);
    tft.setCursor(1 * BOXSIZE + BOXSIZE / 2 - 8, 6 * BOXSIZE + BOXSIZE / 2 - 8);
    tft.println(x);
    tft.fillRect(2 * BOXSIZE, 6 * BOXSIZE, BOXSIZE, BOXSIZE, BLACK);
    tft.setCursor(2 * BOXSIZE + BOXSIZE / 2 - 8, 6 * BOXSIZE + BOXSIZE / 2 - 8);
    tft.println(y);
    tft.fillRect(3 * BOXSIZE, 6 * BOXSIZE, BOXSIZE, BOXSIZE, BLACK);
    tft.setCursor(3 * BOXSIZE + BOXSIZE / 2 - 8, 6 * BOXSIZE + BOXSIZE / 2 - 8);
    tft.println(BINGO[rand_x]);
    tft.fillRect(4 * BOXSIZE, 6 * BOXSIZE, BOXSIZE, BOXSIZE, BLACK);
    tft.setCursor(4 * BOXSIZE + BOXSIZE / 2 - 8, 6 * BOXSIZE + BOXSIZE / 2 - 8);
    tft.println(numbers[rand_x][rand_y]);
  }
}







