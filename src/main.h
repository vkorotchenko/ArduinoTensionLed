
#ifndef TENSION_LED_H_
#define TENSION_LED_H_

#include <Arduino.h>
#include <ArduinoBLE.h>
#include <NeoPixelBus.h>

class TensionLed
{
public:
    void tensionCharacteristicWritten(BLEDevice central, BLECharacteristic characteristic);
protected:
private:  
};


#endif /* TENSION_LED_H_ */