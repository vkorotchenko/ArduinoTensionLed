
#ifndef TENSION_LED_H_
#define TENSION_LED_H_

#include <Arduino.h>
#include <ArduinoBLE.h>
#include <NeoPixelBus.h>
#include "config.h"

class TensionLed
{
public:
    void tensionCharacteristicWritten(BLEDevice central, BLECharacteristic characteristic);
    void display(byte incoming[], int length);
    void displaySingleLed(int holdId, int colourId);
    void clearBoard();


protected:
private:  
};


#endif /* TENSION_LED_H_ */