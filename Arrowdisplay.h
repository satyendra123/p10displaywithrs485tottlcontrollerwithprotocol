// ArrowDisplay.h
#ifndef ARROW_DISPLAY_H
#define ARROW_DISPLAY_H

#include <DMD3.h>

class ArrowDisplay {
public:
    void init();
    void showRightArrows();
    void showLeftArrows();

private:
    static const unsigned int NUM_FRAMES = 3;
    static const unsigned int ADVANCE_MS = 1000 / NUM_FRAMES;
    Bitmap::ProgMem frames[NUM_FRAMES];
};

#endif
