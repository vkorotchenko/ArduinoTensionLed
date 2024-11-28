# 1 "C:\\Users\\VADIMK~1\\AppData\\Local\\Temp\\tmpq_i0h87p"
#include <Arduino.h>
# 1 "C:/workspace/ArduinoTensionLed/src/main.ino"
#include "main.h"

#ifdef GRB
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);
#else
NeoPixelBus<NeoRgbFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);
#endif


const int ledPin = LED_BUILTIN;

BLEService connectionService("4488b571-7806-4df6-bcff-a2897e4953ff");
BLEService ledService("6E400001-B5A3-F393-E0A9-E50E24DCCA9E");

BLECharacteristic tensionCharacteristic("6E400002-B5A3-F393-E0A9-E50E24DCCA9E", BLEWrite | BLENotify, 20, false);
BLEDescriptor tensionDescriptor("00002902-0000-1000-8000-00805f9b34fb", "");

bool isHold = false;
int lastRead = 0;
bool processHolds = false;
void setup();
void tensionCharacteristicWritten(BLEDevice central, BLECharacteristic characteristic);
void display(byte incoming[], int length);
void displaySingleLed(int holdId, int colourId);
void clearBoard();
void loop();
#line 22 "C:/workspace/ArduinoTensionLed/src/main.ino"
void setup() {
  Serial.begin(9600);

  clearBoard();


  strip.SetPixelColor(0, green.colour);
  for (int i = 0; i < PixelCount; i++) {
    strip.ShiftRight(1);
    strip.Show();
    delay(10);
  }
  strip.SetPixelColor(0, blue.colour);
  for (int i = 0; i < PixelCount; i++) {
    strip.ShiftRight(1);
    strip.Show();
    delay(10);
  }
  strip.SetPixelColor(0, pink.colour);
  for (int i = 0; i < PixelCount; i++) {
    strip.ShiftRight(1);
    strip.Show();
    delay(10);
  }
  strip.SetPixelColor(0, red.colour);
  for (int i = 0; i < PixelCount; i++) {
    strip.ShiftRight(1);
    strip.Show();
    delay(10);
  }

  clearBoard();
  strip.Begin();
  strip.Show();


  if (!BLE.begin()) {
    Serial.println("starting Bluetooth® Low Energy module failed!");

    while (1);
  }


  BLE.setLocalName("Tension");

  BLE.setAdvertisedService(connectionService);


  ledService.addCharacteristic(tensionCharacteristic);
  tensionCharacteristic.addDescriptor(tensionDescriptor);


  BLE.addService(connectionService);
  BLE.addService(ledService);

  tensionCharacteristic.setEventHandler(BLEWritten, tensionCharacteristicWritten);


  BLE.advertise();

  Serial.println("Bluetooth® device active, waiting for connections...");
}

void tensionCharacteristicWritten(BLEDevice central, BLECharacteristic characteristic) {
  int length = characteristic.valueLength();
  byte incoming[length];

  characteristic.readValue(incoming, length);

  display(incoming, length);

}

void display(byte incoming[], int length){
  for (int i = 0; i <length; i++) {
    int currentRead = (int)incoming[i];

    if(processHolds) {
      if(!isHold) {
        lastRead = currentRead;
        isHold = true;
      } else {
        displaySingleLed(lastRead, currentRead);
        isHold = false;
      }
    }

    if(currentRead == 1) {
      clearBoard();
    }

    if (currentRead == 3) {
        strip.Show();
        lastRead = 0;
        isHold = false;
        processHolds = false;

    }

    if (currentRead == 80) {
      processHolds = true;
      isHold = false;
    }


    lastRead = currentRead;
  }
}

void displaySingleLed(int holdId, int colourId) {

  colour colour;
  bool isBright;

  for (int i = 0; i < sizeof(colours)/sizeof(colours[0]); i++) {
   if (colourId == colours[i].id) {
     colour = colours[i];
     isBright = false;
     break;
   }
   if (colourId == colours[i].bright_id) {
    colour = colours[i];
    isBright = true;
    break;
   }
  }

  int index;

  if (isBright == true) {
    for (int i = 0; i < sizeof(brightledmapping); i++) {
      if (holdId == brightledmapping[i]) {
        index = i + bright_offset;
        break;
      }
    }
  } else {
    if (holdId < 22) {
      for (int i = 0; i < sizeof(footholdmapping); i++) {
        if (holdId == footholdmapping[i]) {
          index = i + foothold_offset;
          break;
        }
      }
    } else {
      for (int i = 0; i < sizeof(ledmapping); i++) {
        if (holdId == ledmapping[i]) {
          index = i;
          break;
        }
      }
    }

  }


  strip.SetPixelColor(index, colour.colour);
  Serial.print("Lighting up the following: ");
  Serial.print("holdId: ");
  Serial.print(holdId);
  Serial.print(" index: ");
  Serial.print(index);
  Serial.print(" colour: ");
  Serial.println(isBright? colour.bright_name : colour.name);

}

void clearBoard() {
  strip.ClearTo(black);
  strip.Show();
}

void loop() {

  BLEDevice central = BLE.central();


  if (central) {

    Serial.print("Connected to central: ");

    Serial.println(central.address());


    while (central.connected()) {
      BLE.poll();
    }


    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
  }
}