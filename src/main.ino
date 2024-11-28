#include "main.h"

#ifdef GRB
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);
#else
NeoPixelBus<NeoRgbFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);
#endif


const int ledPin = LED_BUILTIN; // set ledPin to on-board LED

BLEService connectionService("4488b571-7806-4df6-bcff-a2897e4953ff"); // create service
BLEService ledService("6E400001-B5A3-F393-E0A9-E50E24DCCA9E"); // create service //UART 

BLECharacteristic tensionCharacteristic("6E400002-B5A3-F393-E0A9-E50E24DCCA9E", BLEWrite | BLENotify, 20, false); //RX characteristic
BLEDescriptor tensionDescriptor("00002902-0000-1000-8000-00805f9b34fb", "");

bool isHold = false;
int lastRead = 0;
bool processHolds = false;

void setup() {
  Serial.begin(9600);

  clearBoard();
  
  // Test LEDs by cycling through the colours and then turning the LEDs off again
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
  strip.Begin(); // Initialize LED strip
  strip.Show(); // Good practice to call Show() in order to clear all LEDs

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting Bluetooth® Low Energy module failed!");

    while (1);
  }

  // set the local name peripheral advertises
  BLE.setLocalName("Tension");
  // set the UUID for the service this peripheral advertises:
  BLE.setAdvertisedService(connectionService);

  // add the characteristics to the service
  ledService.addCharacteristic(tensionCharacteristic);
  tensionCharacteristic.addDescriptor(tensionDescriptor);

  // add the service
  BLE.addService(connectionService);
  BLE.addService(ledService);

  tensionCharacteristic.setEventHandler(BLEWritten, tensionCharacteristicWritten);

  // start advertising
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

    if(processHolds) { // if we are processing holds 
      if(!isHold) {  // if its a hold then store id until we get the next item
        lastRead = currentRead;
        isHold = true;
      } else { // if colour then light up the strip with correct colour
        displaySingleLed(lastRead, currentRead);
        isHold = false;
      }
    }

    if(currentRead == 1) {
      clearBoard();
    }

    if (currentRead == 3) { // end command
        strip.Show();
        lastRead = 0;
        isHold = false;
        processHolds = false;

    }

    if (currentRead == 80) { // hold and colours begin, alternating
      processHolds = true;
      isHold = false;
    }

    //update last read
    lastRead = currentRead;
  }
}

void displaySingleLed(int holdId, int colourId) {
  //get colour
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
    // get index of hold ID:
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

  // light up the appropriate colour
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
  // listen for Bluetooth® Low Energy peripherals to connect:
  BLEDevice central = BLE.central();

  // if a central is connected to peripheral:
  if (central) {

    Serial.print("Connected to central: ");
    // print the central's MAC address:
    Serial.println(central.address());

    // while the central is still connected to peripheral:
    while (central.connected()) {
      BLE.poll();
    }

    // when the central disconnects, print it out:
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
  }
}
  
