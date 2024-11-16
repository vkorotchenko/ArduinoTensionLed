#include <Arduino.h>
#include <ArduinoBLE.h>
#include <tension.h>
#include <NeoPixelBus.h>

// Adjust brightness settings to your personal needs and light situation
#define brightness 64 // 0 to 255; needs to be divisible by 2!


#define RGB // Change this to GRB if your colors are off

const uint8_t PixelPin = 3;  // Adjust only if you don't use pin D2 of Arduino Nano 33 BLE for the data line
const uint16_t PixelCount = 200;
int brightness_adjustment = 10;

RgbColor red_led(brightness, 0, 0);
RgbColor green_led(0, brightness, 0);
RgbColor blue_led(0, 0, brightness);
RgbColor pink_led(brightness, 0, brightness/2);
RgbColor red_led_bright(brightness+brightness_adjustment, 0, 0);
RgbColor green_led_bright(0, brightness+brightness_adjustment, 0);
RgbColor blue_led_bright(0, 0, brightness+brightness_adjustment);
RgbColor pink_led_bright(brightness+brightness_adjustment, 0, (brightness+brightness_adjustment)/2);
RgbColor black(0);

RgbColor rgb_colours[] = {red_led, green_led, blue_led, pink_led, red_led_bright, green_led_bright, blue_led_bright, pink_led_bright, black};



#ifdef GRB
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);
#else
NeoPixelBus<NeoRgbFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);
#endif

BLEService connectionService("4488b571-7806-4df6-bcff-a2897e4953ff"); // create service
BLEService ledService("6E400001-B5A3-F393-E0A9-E50E24DCCA9E");        // create service //UART

BLECharacteristic tensionCharacteristic("6E400002-B5A3-F393-E0A9-E50E24DCCA9E", BLEWrite | BLENotify, 20, false); // RX characteristic
BLEDescriptor tensionDescriptor("00002902-0000-1000-8000-00805f9b34fb", "");

Tension tension;

void setup()
{
  Serial.begin(9600);

  strip.ClearTo(black);
  strip.Show();

  // Test LEDs by cycling through the colours and then turning the LEDs off again
  strip.SetPixelColor(0, rgb_colours[tension.green.colour_index]);
  for (int i = 0; i < PixelCount; i++)
  {
    strip.ShiftRight(1);
    strip.Show();
    delay(10);
  }
  strip.SetPixelColor(0, rgb_colours[tension.blue.colour_index]);
  for (int i = 0; i < PixelCount; i++)
  {
    strip.ShiftRight(1);
    strip.Show();
    delay(10);
  }
  strip.SetPixelColor(0, rgb_colours[tension.pink.colour_index]);
  for (int i = 0; i < PixelCount; i++)
  {
    strip.ShiftRight(1);
    strip.Show();
    delay(10);
  }
  strip.SetPixelColor(0, rgb_colours[tension.red.colour_index]);
  for (int i = 0; i < PixelCount; i++)
  {
    strip.ShiftRight(1);
    strip.Show();
    delay(10);
  }

  strip.Begin(); // Initialize LED strip
  strip.ClearTo(black);
  strip.Show();

  // begin initialization
  if (!BLE.begin())
  {
    Serial.println("starting Bluetooth® Low Energy module failed!");

    while (1)
      ;
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

void tensionCharacteristicWritten(BLEDevice central, BLECharacteristic characteristic)
{
  int length = characteristic.valueLength();
  char incoming[length];

  characteristic.readValue(incoming, length);

  std::vector<Event> events = tension.display(incoming, length);
  for (size_t i = 0; i < events.size(); i++)
  {
    Event currentEvent = events[i];
    if (currentEvent.act == Action::CLEAR)
    {
      strip.ClearTo(black);
      strip.Show();
    }
    else if (currentEvent.act == Action::SHOW)
    {
      strip.Show();
    }
    else if (currentEvent.act == Action::DISPLAY_LED)
    {
      if(currentEvent.isBright)
        strip.SetPixelColor(tension.getBrightColourIndex(currentEvent.holdId), rgb_colours[currentEvent.colourId]);
      else
      strip.SetPixelColor(tension.getColourIndex(currentEvent.holdId), rgb_colours[currentEvent.colourId]);
    }
  }
}
void loop()
{
  // listen for Bluetooth® Low Energy peripherals to connect:
  BLEDevice central = BLE.central();

  // if a central is connected to peripheral:
  if (central)
  {

    Serial.print("Connected to central: ");
    // print the central's MAC address:
    Serial.println(central.address());

    // while the central is still connected to peripheral:
    while (central.connected())
    {
      BLE.poll();
    }

    // when the central disconnects, print it out:
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
  }
}