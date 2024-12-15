
#include <NeoPixelBus.h>

// Adjust brightness settings to your personal needs and light situation
#define brightness 64 // 0 to 255; needs to be divisible by 2!
#define additionalledbrightness 32 // 0 to 255

#define RGB // Change this to GRB if your colors are off

const uint8_t PixelPin = 2;  // Adjust only if you don't use pin D2 of Arduino Nano 33 BLE for the data line
const uint16_t PixelCount = 250;

int ledmapping[] = { // mapping of the hold Ids int he tension board correlating to the index for the strip to light up, hold ID 22 is A1, to ID 39 is A18.. ID 74 is B18 to B1 (57) etc.
      22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,
      74,73,72,71,70,69,68,67,66,65,64,63,62,61,60,59,58,57,
      92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,
      144,143,142,141,140,139,138,137,136,135,134,133,132,131,130,129,128,127,
      162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,
      214,213,212,211,210,209,208,207,206,205,204,203,202,201,200,199,198,197,
      232,233,234,235,236,237,238,239,240,241,242,243,244,245,246,247,248,249,
};

int footholdmapping[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21};

int brightledmapping[] = {
      28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,
      46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,
      98,97,96,95,94,93,92,91,90,89,88,87,86,85,84,83,82,81,
      116,117,118,119,120,121,122,123,124,125,126,127,128,129,130,131,132,133
};

int brightness_adjustment = 1;
int bright_offset = 126;
int foothold_offset= 215;

RgbColor red_led(brightness, 0, 0);
RgbColor green_led(0, brightness, 0);
RgbColor blue_led(0, 0, brightness);
RgbColor pink_led(brightness, 0, brightness/2);
RgbColor red_led_bright(brightness+brightness_adjustment, 0, 0);
RgbColor green_led_bright(0, brightness+brightness_adjustment, 0);
RgbColor blue_led_bright(0, 0, brightness+brightness_adjustment);
RgbColor pink_led_bright(brightness+brightness_adjustment, 0, (brightness+brightness_adjustment)/2);
RgbColor black(0);

struct colour{
    RgbColor colour;
    int id;
    String name;

    RgbColor bright_colour;
    int bright_id;
    String bright_name;
};

colour red = {red_led, 192, "red",red_led_bright, 193, "bright red"};
colour green = {green_led, 48, "green",green_led_bright, 49, "bright green"};
colour blue = {blue_led, 12, "blue",blue_led_bright, 13, "bright blue"};
colour pink = {pink_led, 204, "pink",pink_led_bright, 205, "bright pink"};

colour colours[] = {blue, pink, green, red};
