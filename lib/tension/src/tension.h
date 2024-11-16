#ifndef TENSION_H
#define TENSION_H

#include "../config.h"

#ifdef GRB
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);
#else
NeoPixelBus<NeoRgbFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);
#endif

class Tension {


public:
    bool isHold = false;
    int lastRead = 0;
    bool processHolds = false;

    Tension() {}

    void display(char incoming[], int length);
    void displaySingleLed(int holdId, int colourId);
    void clearBoard();

};

#endif