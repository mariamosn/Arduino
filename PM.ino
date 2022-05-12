#include <Adafruit_NeoPixel.h>

#define LED_PIN    6
#define LED_COUNT 64

#define LEDS_ON_LINE 8
#define LEDS_ON_COL 8

#define LINES 3
#define COLS 3

#define CRT_PLAYER_FACT_MAX 100
#define CRT_PLAYER_FACT_MIN 0

int crt_player;
int crt_player_fact;
int crt_player_dir;

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(9600);
  
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  crt_player = 0;
  crt_player_fact = 0;
  crt_player_dir = 1;
}

void color_border() {
  int r, g, b;
  if (crt_player == 0) {
    r = 1;
    g = 0;
    b = 0;
  }
  // upper border
  for (int i = 0; i < LEDS_ON_COL; i++) {
    strip.setPixelColor(i, r * crt_player_fact, g * crt_player_fact, b * crt_player_fact);
  }
  // lower border
  for (int i = 0; i < LEDS_ON_COL; i++) {
    strip.setPixelColor((LEDS_ON_COL - 1) * LEDS_ON_LINE + i,
                        r * crt_player_fact,
                        g * crt_player_fact,
                        b * crt_player_fact);
  }
  // left border
  for (int i = 0; i < LEDS_ON_LINE; i++) {
    strip.setPixelColor(i * LEDS_ON_LINE,
                        r * crt_player_fact,
                        g * crt_player_fact,
                        b * crt_player_fact);
  }
  // right border
  for (int i = 0; i < LEDS_ON_LINE; i++) {
    strip.setPixelColor(i * LEDS_ON_LINE + LEDS_ON_LINE - 1,
                        r * crt_player_fact,
                        g * crt_player_fact,
                        b * crt_player_fact);
  }
}

void update_crt_player_factor() {
  if (crt_player_fact + crt_player_dir <= CRT_PLAYER_FACT_MAX &&
      crt_player_fact + crt_player_dir >= CRT_PLAYER_FACT_MIN) {
        crt_player_fact += crt_player_dir;
  } else {
    crt_player_dir *= -1;
    crt_player_fact += crt_player_dir;
  }
}

void color_grid() {
 int r, g, b;
  for (int line = 0; line < LINES; line++) {
    for (int col = 0; col < COLS; col++) {
      int line_index = line * 2 + 1;
      int col_index = col * 2 + 1;
      int elem1 = line_index * LEDS_ON_LINE + col_index;
      int elem2 = elem1 + 1;
      int elem3 = elem1 + LEDS_ON_LINE;
      int elem4 = elem3 + 1;
      if ((line + col) % 2 == 0) {
        r = 50;
        g = 0;
        b = 0;
      } else {
        r = 0;
        g = 50;
        b = 0;
      }
      strip.setPixelColor(elem1, r, g, b);
      strip.setPixelColor(elem2, r, g, b);
      strip.setPixelColor(elem3, r, g, b);
      strip.setPixelColor(elem4, r, g, b);
    }
  }
}

void loop() {
  color_border();
  update_crt_player_factor();
  color_grid();

  strip.show();
}
