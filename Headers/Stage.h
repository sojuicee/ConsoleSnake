#pragma once

#include "Point.h"
#include "UI.h"
#include "GridMap.h"

// Enum class ConsoleColor already defined in Game.h
enum class ConsoleColor;

class Stage
{
private:
    UI ui;             // User interface object
    GridMap* gridMap;  // Pointer to the gameplay grid

    void drawStageScreen(); // Draws the stage boundaries and title

public:
    static const ConsoleColor BackgroundColor; // Stage background color
    static const ConsoleColor ForegroundColor; // Stage text color
    static const char BorderCharacter;         // Character used for borders

    Stage();   // Constructor
    ~Stage();  // Destructor
    int run(); // Starts the stage (draws, sets up UI, runs grid)
};
