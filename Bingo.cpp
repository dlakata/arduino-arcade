#include "Arcade.h"

#define BOXSIZE 64
#define BINGO_OFFSET 32

#define NUM_SQUARES 5
#define MIN_NUM 10
#define MAX_NUM 99

#define RESET -2
#define NOT_A_SQUARE -1

int numbers[NUM_SQUARES][NUM_SQUARES];
char BINGO[] = "BINGO";

int rand_x, rand_y;
int bingos_left, game_over;

TouchScreen *ts;
Adafruit_TFTLCD *tft;

Bingo::Bingo(TouchScreen *_ts, Adafruit_TFTLCD *_tft) {
  ts = _ts;
  tft = _tft;
}

int determine_square(TSPoint p, int *box_x, int *box_y) {
  *box_x = p.x / BOXSIZE;
  *box_y = (p.y - BINGO_OFFSET) / BOXSIZE;
  if (p.x < BOXSIZE && p.y > 6 * BOXSIZE) {
    return RESET; 
  }
  if (*box_y > NUM_SQUARES - 1 || *box_x > NUM_SQUARES - 1) {
    *box_x = NOT_A_SQUARE;
    *box_y = NOT_A_SQUARE;
  }
  return 0;
}

void draw_square(int x, int y, int color) {
  tft->fillRect(x * BOXSIZE, BINGO_OFFSET + y * BOXSIZE, BOXSIZE, BOXSIZE, color);
  tft->drawRect(x * BOXSIZE, BINGO_OFFSET + y * BOXSIZE, BOXSIZE, BOXSIZE, WHITE);
}

void write_square(int x, int y, int num) {
  tft->setCursor(x * BOXSIZE + BOXSIZE / 2 - 8, BINGO_OFFSET + y * BOXSIZE + BOXSIZE / 2 - 8);
  tft->println(num);
}

int decrement_bingos() {
  int decrement = random(0, 20);
  if (decrement < 1) {
    return 1; 
  }
  return 0;
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

void draw_bingos_left() {
  tft->fillRect(2 * BOXSIZE, 7 * BOXSIZE, 3 * BOXSIZE, BOXSIZE, BLACK);
  tft->setCursor(2 * BOXSIZE, 7 * BOXSIZE);
  tft->println("Bingos left:");
  tft->setCursor(4 * BOXSIZE + BOXSIZE / 2, 7 * BOXSIZE);
  tft->println(bingos_left);
}

void reset_board() {
  tft->fillScreen(BLACK);
  bingos_left = 50;
  game_over = 0;

  randomSeed(analogRead(0));

  for (int i = 0; i < NUM_SQUARES; i++) {
    for (int j = 0; j < NUM_SQUARES; j++) {
      draw_square(j, i, BLUE);
    }
  }
  tft->setTextColor(WHITE);
  tft->setTextSize(2);

  for (int i = 0; i < NUM_SQUARES; i++) {
    tft->setCursor(i * BOXSIZE + BOXSIZE / 2 - 8, 8);
    tft->println(BINGO[i]);
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
  tft->fillRect(2 * BOXSIZE, 6 * BOXSIZE, BOXSIZE, BOXSIZE, BLACK);
  tft->fillRect(3 * BOXSIZE, 6 * BOXSIZE, BOXSIZE, BOXSIZE, BLACK);
  tft->setCursor(2 * BOXSIZE, 6 * BOXSIZE);
  tft->println(BINGO[rand_x]);
  tft->setCursor(3 * BOXSIZE, 6 * BOXSIZE);
  tft->println(numbers[rand_x][rand_y]);

  tft->setCursor(BOXSIZE / 4, 7 * BOXSIZE);
  tft->println("RESET");
  draw_bingos_left();
}

int victory(int x, int y) {
  int row = 1;
  int col = 1;
  for (int i = 0; i < NUM_SQUARES; i++) {
    if (numbers[x][i] != -1) {
      col = 0; 
    }
    if (numbers[i][y] != -1) {
      row = 0; 
    }
  }
  if (row == 1) {
    for (int i = 0; i < NUM_SQUARES; i++) {
      draw_square(i, y, GREEN);
    }
  } 
  else if (col == 1) {
    for (int i = 0; i < NUM_SQUARES; i++) {
      draw_square(x, i, GREEN);
    }
  }
  return row | col;
}

void Bingo::reset() {
  reset_board();
}

void Bingo::run() {
  TSPoint p = ts->getPoint();

  // if sharing pins, you'll need to fix the directions of the touchscreen pins
  //pinMode(XP, OUTPUT);
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  //pinMode(YM, OUTPUT);

  int pressed = p.z > MINPRESSURE && p.z < MAXPRESSURE;

  if (pressed) {
    // scale from 0->1023 to tft->width
    p.x = map(p.x, TS_MINX, TS_MAXX, tft->width(), 0);
    p.y = map(p.y, TS_MINY, TS_MAXY, tft->height(), 0);

    int x, y;
    if (determine_square(p, &x, &y) == RESET) {
      reset_board();
    } 
    else if (!game_over) {
      if (x == rand_x && y == rand_y) {
        numbers[rand_x][rand_y] = -1;
        draw_square(x, y, RED);
        get_random_square();
        tft->fillRect(2 * BOXSIZE, 6 * BOXSIZE, BOXSIZE, BOXSIZE, BLACK);
        tft->fillRect(3 * BOXSIZE, 6 * BOXSIZE, BOXSIZE, BOXSIZE, BLACK);
        tft->setCursor(2 * BOXSIZE, 6 * BOXSIZE);
        tft->println(BINGO[rand_x]);
        tft->setCursor(3 * BOXSIZE, 6 * BOXSIZE);
        tft->println(numbers[rand_x][rand_y]);
      }

      if (bingos_left <= 0) {
        tft->fillRect(2 * BOXSIZE, 7 * BOXSIZE, 3 * BOXSIZE, BOXSIZE, BLACK);
        tft->setCursor(3 * BOXSIZE, 7 * BOXSIZE);
        tft->println("Game over!");
        game_over = 1;
      } 
      else if (decrement_bingos()) {
        bingos_left--;
        draw_bingos_left();
      }
      if (victory(x, y)) {
        tft->fillRect(2 * BOXSIZE, 7 * BOXSIZE, 3 * BOXSIZE, BOXSIZE, BLACK);
        tft->setCursor(3 * BOXSIZE, 7 * BOXSIZE);
        tft->println("You won!");
        game_over = 1;
      }
    }
  }
}


