#include <Adafruit_NeoPixel.h>
#include <LiquidCrystal.h>

#define LED_PIN    6
#define LED_COUNT 64

#define LEDS_ON_LINE 8
#define LEDS_ON_COL 8

#define LINES 3
#define COLS 3

#define CRT_PLAYER_FACT_MAX 50
#define CRT_PLAYER_FACT_MIN 0

#define BUTTON_LEFT 9
#define BUTTON_OK 8
#define BUTTON_RIGHT 7
#define BUTTON_COOLDOWN 200

int button_cooldown;

#define P1 0
#define P2 1
#define EMPTY 2

int crt_player;
int crt_player_fact;
int crt_player_dir;
int crt_line;
int crt_col;

int col_players[2];
int grid[LINES][COLS] = {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY};

char col[6][8] = {"red    ", "green  ", "blue   ", "yellow ", "magenta", "cyan   "};
int rgb[6][3] = { 1, 0, 0, // red
                  0, 1, 0, // green
                  0, 0, 1, // blue
                  1, 1, 0, // yellow
                  1, 0, 1, // magenta
                  0, 1, 1}; // cyan

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void random_col() {
  int r = 50, g = 0, b = 0;
  int next = 0;

  for (int i = 1; i <= 50; i++) {
    for (int j = 0; j < LED_COUNT; j++)
      strip.setPixelColor(j, r, g, b);
    strip.show();
    delay(100);
    r--;
    b++;
  }
  for (int i = 1; i <= 50; i++) {
    for (int j = 0; j < LED_COUNT; j++)
      strip.setPixelColor(j, r, g, b);
    strip.show();
    delay(100);
    b--;
    g++;
  }
  for (int i = 1; i <= 50; i++) {
    for (int j = 0; j < LED_COUNT; j++)
      strip.setPixelColor(j, r, g, b);
    strip.show();
    delay(100);
    g--;
    r++;
  }
}

void pick_color() {
  for (int i = 0; i < 2; i++) {
    col_players[i] = 0;
    lcd.clear();
    lcd.print(String("Player ") + String(i + 1) + String(" color:"));

    int crt_col = 0;
    if (i == 1 && crt_col == col_players[0]) {
      crt_col = (crt_col - 1 + 6) % 6;
    }
    while (1) {
      lcd.setCursor(0, 1);
      lcd.print(col[crt_col]);
      
      if (digitalRead(BUTTON_OK) == 0) {
        col_players[i] = crt_col;
        delay(BUTTON_COOLDOWN);
        break;
      }
      if (digitalRead(BUTTON_LEFT) == 0) {
        crt_col = (crt_col - 1 + 6) % 6;
        if (i == 1 && crt_col == col_players[0]) {
          crt_col = (crt_col - 1 + 6) % 6;
        }
      }
      if (digitalRead(BUTTON_RIGHT) == 0) {
        crt_col = (crt_col + 1) % 6;
        if (i == 1 && crt_col == col_players[0]) {
          crt_col = (crt_col + 1) % 6;
        }
      }
      delay(BUTTON_COOLDOWN);
    }
  }
}

void game_start() {
  lcd.clear();
  lcd.print("Game starts!");
  lcd.setCursor(0, 1);
  lcd.print("Good luck!");
  delay(3000);
}

void restart() {
  // led matrix setup
  // Initialize all pixels to 'off'
  for (int j = 0; j < LED_COUNT; j++)
      strip.setPixelColor(j, 0, 0, 0);
  strip.show();

  // game setup
  crt_player = P1;
  crt_player_fact = 0;
  crt_player_dir = 1;
  crt_line = 0;
  crt_col = 0;

  for (int i = 0; i < LINES; i++)
    for (int j = 0; j < COLS; j++)
      grid[i][j] = EMPTY;

  button_cooldown = 0;
  
  pick_color();
  game_start();
}

void setup() {
  // used for debugging
  Serial.begin(9600);

  // led matrix setup
  // Initialize all pixels to 'off'
  strip.begin();
  strip.show();

  // game setup
  crt_player = P1;
  crt_player_fact = 0;
  crt_player_dir = 1;
  crt_line = 0;
  crt_col = 0;

  // lcd display setup
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("Hello!");

  // buttons setup
  pinMode(BUTTON_LEFT, INPUT_PULLUP);
  pinMode(BUTTON_OK, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT, INPUT_PULLUP);
  button_cooldown = 0;
  
  pick_color();
  game_start();
}

void color_border() {
  int r, g, b;
  
  r = rgb[col_players[crt_player]][0];
  g = rgb[col_players[crt_player]][1];
  b = rgb[col_players[crt_player]][2];

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
  for (int i = 1; i < LEDS_ON_LINE; i++) {
    strip.setPixelColor(i * LEDS_ON_LINE,
                        r * crt_player_fact,
                        g * crt_player_fact,
                        b * crt_player_fact);
  }
  // right border
  for (int i = 1; i < LEDS_ON_LINE; i++) {
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
      
      if (grid[line][col] == P1) {
        r = 20 * rgb[col_players[P1]][0];
        g = 20 * rgb[col_players[P1]][1];
        b = 20 * rgb[col_players[P1]][2];
      } else if (grid[line][col] == P2){
        r = 20 * rgb[col_players[P2]][0];
        g = 20 * rgb[col_players[P2]][1];
        b = 20 * rgb[col_players[P2]][2];
      } else {
        r = g = b = 0;
      }
      strip.setPixelColor(elem1, r, g, b);
      strip.setPixelColor(elem2, r, g, b);
      strip.setPixelColor(elem3, r, g, b);
      strip.setPixelColor(elem4, r, g, b);
    }
  }
}

void right_push() {
  crt_col++;
  if (crt_col == COLS) {
    crt_col = 0;
    crt_line++;
  }
  if (crt_line == LINES) {
    crt_line = 0;
  }
}

void left_push() {
  crt_col--;
  if (crt_col == -1) {
    crt_col = COLS - 1;
    crt_line--;
  }
  if (crt_line == -1) {
    crt_line = LINES - 1;
  }
}

void switch_crt_player() {
  if (crt_player == P1) {
    crt_player = P2;
  } else {
    crt_player = P1;
  }
}

void ok_push() {
  if (grid[crt_line][crt_col] == EMPTY) {
    grid[crt_line][crt_col] = crt_player;
    switch_crt_player();
    crt_line = 0;
    crt_col = 0;
  }
}

void blink_crt() {
  int line_index = crt_line * 2 + 1;
  int col_index = crt_col * 2 + 1;
  int elem1 = line_index * LEDS_ON_LINE + col_index;
  int elem2 = elem1 + 1;
  int elem3 = elem1 + LEDS_ON_LINE;
  int elem4 = elem3 + 1;

  strip.setPixelColor(elem1, crt_player_fact, crt_player_fact, crt_player_fact);
  strip.setPixelColor(elem2, crt_player_fact, crt_player_fact, crt_player_fact);
  strip.setPixelColor(elem3, crt_player_fact, crt_player_fact, crt_player_fact);
  strip.setPixelColor(elem4, crt_player_fact, crt_player_fact, crt_player_fact);
}

void celebrate(int winner) {
  lcd.clear();
  lcd.print(String("Player ") + String(winner + 1) + String(" wins!"));

  for (int i = 0; i < LED_COUNT; i++)
    strip.setPixelColor(i, 0, 0, 0);

  int r = rgb[col_players[winner]][0] * 20;
  int g = rgb[col_players[winner]][1] * 20;
  int b = rgb[col_players[winner]][2] * 20;

  // drawing
  strip.setPixelColor(2, r, g, b);
  strip.setPixelColor(3, r, g, b);
  strip.setPixelColor(4, r, g, b);
  strip.setPixelColor(5, r, g, b);
  
  strip.setPixelColor(9, r, g, b);
  strip.setPixelColor(14, r, g, b);
  
  strip.setPixelColor(16, r, g, b);
  strip.setPixelColor(18, r, g, b);
  strip.setPixelColor(21, r, g, b);
  strip.setPixelColor(23, r, g, b);
  
  strip.setPixelColor(24, r, g, b);
  // strip.setPixelColor(26, r, g, b);
  // strip.setPixelColor(28, r, g, b);
  strip.setPixelColor(31, r, g, b);
  
  strip.setPixelColor(32, r, g, b);
  strip.setPixelColor(34, r, g, b);
  strip.setPixelColor(37, r, g, b);
  strip.setPixelColor(39, r, g, b);
  
  strip.setPixelColor(40, r, g, b);
  // strip.setPixelColor(42, r, g, b);
  strip.setPixelColor(43, r, g, b);
  strip.setPixelColor(44, r, g, b);
  strip.setPixelColor(47, r, g, b);
  
  strip.setPixelColor(49, r, g, b);
  strip.setPixelColor(54, r, g, b);
  
  strip.setPixelColor(58, r, g, b);
  strip.setPixelColor(59, r, g, b);
  strip.setPixelColor(60, r, g, b);
  strip.setPixelColor(61, r, g, b);
  /*
  strip.setPixelColor(9, r, g, b);
  strip.setPixelColor(10, r, g, b);
  strip.setPixelColor(17, r, g, b);
  strip.setPixelColor(18, r, g, b);

  strip.setPixelColor(13, r, g, b);
  strip.setPixelColor(14, r, g, b);
  strip.setPixelColor(21, r, g, b);
  strip.setPixelColor(22, r, g, b);

  strip.setPixelColor(33, r, g, b);
  strip.setPixelColor(41, r, g, b);
  strip.setPixelColor(42, r, g, b);
  strip.setPixelColor(50, r, g, b);
  strip.setPixelColor(51, r, g, b);
  strip.setPixelColor(52, r, g, b);
  strip.setPixelColor(53, r, g, b);
  strip.setPixelColor(45, r, g, b);
  strip.setPixelColor(46, r, g, b);
  strip.setPixelColor(38, r, g, b);
  */

  strip.show();
  delay(10000);

  // random_col();
}

void draw() {
  lcd.clear();
  lcd.print("Game over!");
  lcd.setCursor(0, 1);
  lcd.print("Draw");

  // drawing
  int r1 = rgb[col_players[0]][0];
  int r2 = rgb[col_players[1]][0] * 30;
  int g1 = rgb[col_players[0]][1];
  int g2 = rgb[col_players[1]][1] * 30;
  int b1 = rgb[col_players[0]][2];
  int b2 = rgb[col_players[1]][2] * 30;

  for (int i = 0; i < LED_COUNT; i++)
    strip.setPixelColor(i, 0, 0, 0);
  
  strip.setPixelColor(0, r2, g2, b2);
  strip.setPixelColor(7, r2, g2, b2);
  
  strip.setPixelColor(9, r2, g2, b2);
  strip.setPixelColor(14, r2, g2, b2);

  strip.setPixelColor(18, r2, g2, b2);
  strip.setPixelColor(19, r2, g2, b2);
  strip.setPixelColor(20, r2, g2, b2);
  strip.setPixelColor(21, r2, g2, b2);
  
  strip.setPixelColor(25, r2, g2, b2);
  strip.setPixelColor(27, r2, g2, b2);
  strip.setPixelColor(28, r2, g2, b2);
  strip.setPixelColor(30, r2, g2, b2);

  strip.setPixelColor(34, r2, g2, b2);
  strip.setPixelColor(35, r2, g2, b2);
  strip.setPixelColor(36, r2, g2, b2);
  strip.setPixelColor(37, r2, g2, b2);

  strip.setPixelColor(40, r2, g2, b2);
  strip.setPixelColor(41, r2, g2, b2);
  strip.setPixelColor(42, r2, g2, b2);
  strip.setPixelColor(43, r2, g2, b2);
  strip.setPixelColor(44, r2, g2, b2);
  strip.setPixelColor(45, r2, g2, b2);
  strip.setPixelColor(46, r2, g2, b2);
  strip.setPixelColor(47, r2, g2, b2);
  
  strip.setPixelColor(48, r2, g2, b2);
  strip.setPixelColor(50, r2, g2, b2);
  strip.setPixelColor(53, r2, g2, b2);
  strip.setPixelColor(55, r2, g2, b2);

  strip.setPixelColor(56, r2, g2, b2);
  strip.setPixelColor(58, r2, g2, b2);
  strip.setPixelColor(59, r2, g2, b2);
  strip.setPixelColor(60, r2, g2, b2);
  strip.setPixelColor(61, r2, g2, b2);
  strip.setPixelColor(63, r2, g2, b2);

  strip.show();
  delay(10000); 

  // random_col();
}

void check_for_winner() {
  int found = -1;
  int done = 1;
  
  // check lines
  for (int i = 0; i < LINES && found == -1; i++) {
    if (grid[i][0] != EMPTY && grid[i][0] == grid[i][1] && grid[i][0] == grid[i][2])
      found = grid[i][0];
  }

  // check cols
  for (int i = 0; i < COLS && found == -1; i++) {
    if (grid[0][i] != EMPTY && grid[0][i] == grid[1][i] && grid[0][i] == grid[2][i])
      found = grid[0][i];
  }

  // check diags
  if (found == -1 && grid[0][0] != EMPTY && grid[0][0] == grid[1][1] && grid[0][0] == grid[2][2])
    found = grid[0][0];
  if (found == -1 && grid[0][2] != EMPTY && grid[0][2] == grid[1][1] && grid[0][2] == grid[2][0])
    found = grid[0][2];

  if (found != -1) {
    celebrate(found);
    restart();
  } else {
    for (int i = 0; i < LINES; i++) {
      for (int j = 0; j < COLS; j++) {
        if (grid[i][j] == EMPTY) {
          done = 0;
        }
      }
    }

    if (done) {
      draw();
      restart();
    }
  }
}

void loop() {
  if (button_cooldown <= 0 && digitalRead(BUTTON_LEFT) == 0) {
    Serial.println("left");
    left_push();
    button_cooldown = BUTTON_COOLDOWN;
  }
  if (button_cooldown <= 0 && digitalRead(BUTTON_OK) == 0) {
    Serial.println("ok");
    ok_push();
    button_cooldown = BUTTON_COOLDOWN;
  }
  if (button_cooldown <= 0 && digitalRead(BUTTON_RIGHT) == 0) {
    Serial.println("right");
    right_push();
    button_cooldown = BUTTON_COOLDOWN;
  }
  
  color_border();
  update_crt_player_factor();
  color_grid();
  blink_crt();

  strip.show();

  delay(50);
  if (button_cooldown > 0) {
    button_cooldown -= 50;
  }

  check_for_winner();
}
