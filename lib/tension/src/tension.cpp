#include <tension.h>

int ledmapping[] = { // mapping of the hold Ids int he tension board correlating to the index for the strip to light up, hold ID 22 is A1, to ID 39 is A18.. ID 74 is B18 to B1 (57) etc.
    22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
    74, 73, 72, 71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57,
    92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109,
    144, 143, 142, 141, 140, 139, 138, 137, 136, 135, 134, 133, 132, 131, 130, 129, 128, 127,
    162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179,
    214, 213, 212, 211, 210, 209, 208, 207, 206, 205, 204, 203, 202, 201, 200, 199, 198, 197,
    232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249};

int brightledmapping[] = {
    28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11,
    46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
    98, 97, 96, 95, 94, 93, 92, 91, 90, 89, 88, 87, 86, 85, 84, 83, 82, 81,
    116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133};

bool isHold = false;
int lastRead = 0;
bool processHolds = false;

int bright_offset = 126;
inline std::vector<Event> Tension::display(char incoming[], int length)
{
  bool isHold = false;
  int lastRead = 0;
  bool processHolds = false;
  std::vector<Event> events;

  for (int i = 0; i < length; i++)
  {
    int currentRead = (int)incoming[i];

    if (processHolds)
    { // if we are processing holds
      if (!isHold)
      { // if its a hold then store id until we get the next item
        lastRead = currentRead;
        isHold = true;
      }
      else
      { // if colour then light up the strip with correct colour
        events.push_back(displaySingleLed(lastRead, currentRead));
        isHold = false;
      }
    }

    if (currentRead == 1)
    {
      Event event(CLEAR, 0, 0);
      events.push_back(event);
    }

    if (currentRead == 3)
    { // end command
      Event event(SHOW, 0, 0);
      events.push_back(event);
      lastRead = 0;
      isHold = false;
      processHolds = false;
    }

    if (currentRead == 80)
    { // hold and colours begin, alternating
      processHolds = true;
      isHold = false;
    }

    // update last read
    lastRead = currentRead;
  }
  return events;
}

inline Event Tension::displaySingleLed(int holdId, int colourId)
{
  // get colour
  int colour = 0;
  bool isBright = false;

  for (size_t i = 0; i < sizeof(colours) / sizeof(colours[0]); i++)
  {
    if (colourId == colours[i].colour_index)
    {
      colour = colours[i].id;
      isBright = false;
    }
    if (colourId == colours[i].bright_colour_index)
    {
      colour = colours[i].bright_id;
      isBright = true;
    }
  }

  int index = 0;
  // get index of hold ID:
  if (isBright == true)
  {
    for (size_t i = 0; i < sizeof(brightledmapping) / sizeof(brightledmapping[0]); i++)
    {
      if (holdId == brightledmapping[i])
      {
        index = i + bright_offset;
        break;
      }
    }
  }
  else
  {
    for (size_t i = 0; i < sizeof(ledmapping) / sizeof(ledmapping[0]); i++)
    {
      if (holdId == ledmapping[i])
      {
        index = i;
        break;
      }
    }
  }

  Event event(DISPLAY_LED, index, colour);
  return event;
}
int getColourIndex(int colourId)
{
  return 0;
};

inline int Tension::getColourIndex(int colourId)
{
  return ledmapping[colourId];
}

inline int Tension::getBrightColourIndex(int colourId)
{
  return brightledmapping[colourId];
}
