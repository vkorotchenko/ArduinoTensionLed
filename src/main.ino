#include "main.h"
#include <tension.h>

BLEService connectionService("4488b571-7806-4df6-bcff-a2897e4953ff"); // create service
BLEService ledService("6E400001-B5A3-F393-E0A9-E50E24DCCA9E"); // create service //UART 

BLECharacteristic tensionCharacteristic("6E400002-B5A3-F393-E0A9-E50E24DCCA9E", BLEWrite | BLENotify, 20, false); //RX characteristic
BLEDescriptor tensionDescriptor("00002902-0000-1000-8000-00805f9b34fb", "");


Tension tension;

void setup() {
  Serial.begin(9600);

  tension.clearBoard();
  
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

  tension.clearBoard();
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
  char incoming[length];

  characteristic.readValue(incoming, length);

  tension.display(incoming, length);
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
  
