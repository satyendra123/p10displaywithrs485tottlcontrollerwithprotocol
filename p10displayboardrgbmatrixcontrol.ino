
#include <RGBmatrixPanel.h>
#define CLK  8   // USE THIS ON ARDUINO UNO, ADAFRUIT METRO M0, etc.
//#define CLK A4 // USE THIS ON METRO M4 (not M0)
//#define CLK 11 // USE THIS ON ARDUINO MEGA
#define OE   9
#define LAT 10
#define A   A0
#define B   A1
#define C   A2

RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);

void setup() {
  matrix.begin();
  uint8_t r=0, g=0, b=0;

  // Draw top half
  for(uint8_t x=0; x<32; x++) {
    for(uint8_t y=0; y<8; y++) {
      matrix.drawPixel(x, y, matrix.Color333(r, g, b));
      r++;
      if(r == 8) {
        r = 0;
        g++;
        if(g == 8) {
          g = 0;
          b++;
        }
      }
    }
  }

  // Draw bottom half
  for(uint8_t x=0; x<32; x++) {
    for(uint8_t y=8; y<16; y++) {
      matrix.drawPixel(x, y, matrix.Color333(r, g, b));
      r++;
      if(r == 8) {
        r = 0;
        g++;
        if(g == 8) {
          g = 0;
          b++;
        }
      }
    }
  }
}

void loop() {
  // Do nothing -- image doesn't change
}
