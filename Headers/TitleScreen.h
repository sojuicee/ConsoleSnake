#pragma once

#include <vector>
#include "Point.h"

using namespace std;

class TitleScreen
{
private:
    Point playerChoiceArrow;          // Arrow position for selecting menu
    bool showEnterMessage;            // Toggle Enter message blink
    bool showChoiceArrow;             // Toggle arrow blink
    Point blinkMsgPosition;           // Position of "Press Enter" message
    vector<short> titleScreenBackColors; // List of background colors for title screen
    short numberLimit;                // Number of colors in the list
    short lastRandom;                 // Last color index used

    // Timer callback functions
    static void blinkPressEnterMsg_callBack(void* ownerObject);
    static void blinkChoiceArrow_callBack(void* ownerObject);
    static void changeTitleScreenColors_callBack(void* ownerObject);

    // Internal methods
    void drawTitleScreen();           // Draw the entire title screen
    void blinkPressEnterMsg();        // Blink "Press Enter" message
    void blinkChoiceArrow();          // Blink selection arrow
    void changeTitleScreenColors();   // Change background color randomly
    void setsTitleScreenColors();     // Apply color to console

public:
    TitleScreen();
    ~TitleScreen();

    int prepareTitleScreen();         // Draws screen and starts timers
    bool waitingForPlayerChoice();    // Waits for arrow/Enter input
    short getHexaColorsCode();        // Get last color code used
};
