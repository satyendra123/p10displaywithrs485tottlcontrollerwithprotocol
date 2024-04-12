// ArrowDisplay.cpp
#include "ArrowDisplay.h"

DMD3 display;

byte const rightArrow1[] PROGMEM = {
    16, 16,
     B00000000, B00000000, 
     B00000000, B00000000,
     B00000001, B00000000,
     B00000011, B00000000,
     B00000111, B00000000,
     B00001111, B00000000,
     B00011111, B11111111,
     B00111111, B11111111,
     B01111111, B11111111,
     B01111111, B11111111,
     B00111111, B00000000,
     B00011111, B00000000,
     B00001111, B00000000,
     B00000111, B00000000,
     B00000011, B00000000,
     B00000000, B00000000,
     B00000000, B00000000,
     B00000000, B00000000,
     B00000000, B00000000,
     B00000000, B00000000,
     B00000000, B00000000,
     B00000000, B00000000
};


byte const rightArrow2[] PROGMEM = {
    16, 16,
    B00000000, B00000000,
    B00000000, B00000000, 
    B00000000, B00000000,
    B00000000, B00000000, 
    B00000000, B00000000,
    B00000000, B00000000,
    B00000000, B00000000,
    B00000000, B00000000,
    B00000000, B00000000,
    B00000000, B00000000,
    B00000000, B00000000,
    B00000000, B00000000,
    B00000000, B00000000,
    B00000000, B00000000,
    B00000000, B00000000,
    B00000000, B00000000,
    B00000000, B00000000,
    B00000000, B00000000,
    B00000000, B00000000,
    B00000000, B00000000,
    B00000000, B00000000,
    B00000000, B00000000
};

byte const rightArrow3[] PROGMEM = {
   B00000000, B00000000, 
     B00000000, B00000000,
     B00000001, B00000000,
     B00000011, B00000000,
     B00000111, B00000000,
     B00001111, B00000000,
     B00011111, B11111111,
     B00111111, B11111111,
     B01111111, B11111111,
     B01111111, B11111111,
     B00111111, B00000000,
     B00011111, B00000000,
     B00001111, B00000000,
     B00000111, B00000000,
     B00000011, B00000000,
     B00000000, B00000000,
     B00000000, B00000000,
     B00000000, B00000000,
     B00000000, B00000000,
     B00000000, B00000000,
     B00000000, B00000000,
     B00000000, B00000000
};


Bitmap::ProgMem rightArrowFrames[] = {
    rightArrow1,
    rightArrow2,
    rightArrow3,
};

byte const leftArrow1[] PROGMEM = {
    16, 16,
    B00000000, B00000000,
    B00000000, B00000000,
    B00000000, B10000000,
    B00000000, B11000000, 
    B00000000, B11100000,
    B00000000, B11110000,
    B11111111, B11111000,
    B11111111, B11111100, 
    B11111111, B11111110,
    B11111111, B11111110, 
    B00000000, B11111100,
    B00000000, B11111000,
    B00000000, B11110000,
    B00000000, B11100000,
    B00000000, B11000000,
    B00000000, B00000000,
    B00000000, B00000000,
    B00000000, B00000000,
    B00000000, B00000000,  
    B00000000, B00000000, 
    B00000000, B00000000,  
    B00000000, B00000000
};


byte const leftArrow2[] PROGMEM = {
    16, 16,
    B00000000, B00000000,
    B00000000, B00000000, 
    B00000000, B00000000,
    B00000000, B00000000, 
    B00000000, B00000000,
    B00000000, B00000000,
    B00000000, B00000000,
    B00000000, B00000000,
    B00000000, B00000000,
    B00000000, B00000000,
    B00000000, B00000000,
    B00000000, B00000000,
    B00000000, B00000000,
    B00000000, B00000000,
    B00000000, B00000000,
    B00000000, B00000000,
    B00000000, B00000000,
    B00000000, B00000000,
    B00000000, B00000000,
    B00000000, B00000000,
    B00000000, B00000000,
    B00000000, B00000000
};

byte const leftArrow3[] PROGMEM = {
    16, 16,
    B00000000, B00000000,
    B00000000, B00000000,
    B00000000, B10000000,
    B00000000, B11000000, 
    B00000000, B11100000,
    B00000000, B11110000,
    B11111111, B11111000,
    B11111111, B11111100, 
    B11111111, B11111110,
    B11111111, B11111110, 
    B00000000, B11111100,
    B00000000, B11111000,
    B00000000, B11110000,
    B00000000, B11100000,
    B00000000, B11000000,
    B00000000, B00000000,
    B00000000, B00000000,
    B00000000, B00000000,
    B00000000, B00000000,  
    B00000000, B00000000, 
    B00000000, B00000000,  
    B00000000, B00000000
};

Bitmap::ProgMem leftArrowFrames[] = {
    leftArrow1,
    leftArrow2,
    leftArrow3,
};

unsigned int rightFrame = 0;
unsigned int leftFrame = 0;

unsigned int frame = 0;

void scan() {
    display.refresh();
}

void ArrowDisplay::init() {
    Timer1.initialize(2000);
    Timer1.attachInterrupt(scan);
    Timer1.pwm(9, 10);
}

void ArrowDisplay::showRightArrows() {
    display.clear();
    int x = 16;  // Always start from the first column
    display.drawBitmap(x, 0, rightArrowFrames[rightFrame]);
    delay(ADVANCE_MS);
    display.refresh();

    rightFrame = (rightFrame + 1) % (sizeof(rightArrowFrames) / sizeof(rightArrowFrames[0]));
}


void ArrowDisplay::showLeftArrows() {
    display.clear();
    int x = 0;  // Always start from the first column
    display.drawBitmap(x, 0, leftArrowFrames[leftFrame]);
    delay(ADVANCE_MS);
    display.refresh();

    leftFrame = (leftFrame + 1) % (sizeof(leftArrowFrames) / sizeof(leftArrowFrames[0]));
}

