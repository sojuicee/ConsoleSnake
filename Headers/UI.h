#pragma once

#include <vector>
#include "Point.h"

using namespace std; // Use standard namespace in all files

enum class ConsoleColor; // Forward declaration for colors

typedef unsigned short ushort;
typedef unsigned char uchar;

class UI
{
private:
    static const vector<ushort> SnakeSpeedTimes;  // Speed thresholds for snake
    const ConsoleColor BackgroundColor;           // Background of UI
    const ConsoleColor ScoreForegroundColor;      // Color of score text
    const ConsoleColor HiScoreForegroundColor;    // Color of high score text
    const ConsoleColor SpeedForegroundColor;      // Color of speed display
    const short PointsToIncreaseSpeed;            // Points needed to increase speed
    bool showScorePoints;                         // Toggle score blinking
    bool showHiScorePoints;                       // Toggle high score blinking
    bool showSpeedValue;                          // Toggle speed blinking
    Point scorePanelPosition;                     // Position of score panel
    Point hiScorePanelPosition;                   // Position of high score panel
    Point snakeSpeedPanelPosition;                // Position of speed panel
    ushort scorePanelPoints;                      // Current score
    ushort nextScoreValue;                        // Next score value for animation
    ushort hiScorePanelPoints;                    // Current high score
    ushort snakeSpeedPanelValue;                  // Current speed value
    ushort speedTimeIndex;                         // Index of speed thresholds
    ushort nextSnakeSpeedTime;                     // Next snake speed value
    vector<void (*)(void*)> blinkMethods;         // List of methods for blinking

    // Internal timer callbacks
    static void blinkScore_callBack(void* ownerObject);
    static void blinkHiScore_callBack(void* ownerObject);
    static void blinkSpeedValue_callBack(void* ownerObject);
    static void prepareToStopBlinking_callBack(void* ownerObject);
    static void stopBlinking_callBack(void* ownerObject);

    // Internal drawing and logic
    void drawUI();
    void blinkPanelValue(bool& showPanelValue, const ConsoleColor& valueForegroundColor);
    void writePanelValue(const Point& panelPosition, const uchar setW, const ushort& panelValue);
    void writeAllPanelValues();
    void prepareToStopBlinking();
    void stopBlinking();

public:
    UI();
    void setupUI();                        // Draw initial UI
    void addScorePoints(ushort pointsToAdd);// Add points and handle blinking
    ushort getNextSpeedPanelValue();       // Get next speed threshold
    ushort getMinimumTimeSpeed();          // Get minimum speed value
    void deleteUItimers();                 // Stop all blinking timers
};
