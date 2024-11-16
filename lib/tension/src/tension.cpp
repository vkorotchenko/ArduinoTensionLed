
#include <tension.h>

void Tension::display(char incoming[], int length){
    bool isHold = false;
    int lastRead = 0;
    bool processHolds = false;
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


void Tension::displaySingleLed(int holdId, int colourId) {
  //get colour
  colour colour;
  bool isBright = false;

  for (int i = 0; i < sizeof(colours); i++) {
   if (colourId == colours[i].id) {
     colour = colours[i];
     isBright = false;
   }
   if (colourId == colours[i].bright_id) {
    colour = colours[i];
    isBright = true;
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
    for (int i = 0; i < sizeof(ledmapping); i++) {
      if (holdId == ledmapping[i]) {
        index = i;
        break;
      }
    }
  }
  
  // light up the appropriate colour
  strip.SetPixelColor(index, colour.colour);
};
  

void Tension::clearBoard() {
  strip.ClearTo(black);
  strip.Show();
}