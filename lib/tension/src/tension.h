#ifndef TENSION_H
#define TENSION_H
#include <vector>
#include <string>

enum Action
{
    CLEAR,
    SHOW,
    DISPLAY_LED
};

class Event
{
public:
    Event(Action act, int holdId, int colourId) : act(act), holdId(holdId), colourId(colourId), isBright(false) {}
    Event(Action act, int holdId, int colourId, bool isBright) : act(act), holdId(holdId), colourId(colourId), isBright(isBright) {}
    Action act;
    int holdId;
    int colourId;
    bool isBright;
};

class Colour
{

public:
    Colour() : colour_index(0), id(0), name(""), bright_colour_index(0), bright_id(0), bright_name("") {}
    Colour(int colour_index, int id, std::string name, int bright_colour_index, int bright_id, std::string bright_name) : colour_index(colour_index), id(id), name(name), bright_colour_index(bright_colour_index), bright_id(bright_id), bright_name(bright_name) {}
    int colour_index;
    int id;
    std::string name;

    int bright_colour_index;
    int bright_id;
    std::string bright_name;
};

class Tension
{

public:

    Colour red;
    Colour green;
    Colour blue;
    Colour pink;

    Colour colours[4];

    Tension() : red(0, 192, "red", 4, 193, "bright red"),
                green(1, 48, "green", 5, 49, "bright green"),
                blue(2, 12, "blue", 6, 13, "bright blue"),
                pink(3, 204, "pink", 7, 205, "bright pink")
    {
        colours[0] = red;
        colours[1] = green;
        colours[2] = blue;
        colours[3] = pink;
    }

    std::vector<Event> display(char incoming[], int length);
    Event displaySingleLed(int holdId, int colourId);
    int getColourIndex(int colourId);
    int getBrightColourIndex(int colourId);
};

#endif