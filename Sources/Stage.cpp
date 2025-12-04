#include <iostream>
#include <iomanip>
#include "../Headers/Stage.h" // Fixed include path
#include "../Headers/Game.h"
#include "../Headers/UI.h"
#include "../Headers/GridMap.h"

using namespace std;

// FIXED: Changed Colors to match Game.h definitions
const ConsoleColor Stage::BackgroundColor = ConsoleColor::White; // Was Gray
const ConsoleColor Stage::ForegroundColor = ConsoleColor::Magenta; // Was Purple
const char Stage::BorderCharacter = '|';

// Constructor: initialize gridMap to nullptr
Stage::Stage()
{
    gridMap = nullptr;
}

// Destructor: free allocated memory
Stage::~Stage()
{
    delete gridMap;
    gridMap = nullptr;
}

// Run the stage: draw screen, setup UI, create grid
int Stage::run()
{
    drawStageScreen(); // Draw the stage borders and title
    ui.setupUI();      // Draw UI panels

    gridMap = new GridMap(ui); // Create grid map for gameplay

    if (gridMap != nullptr)
    {
        gridMap->drawGrid(); // Draw the grid
        gridMap->run();      // Start gameplay loop
    }

    return 1; // BACKTOSTART equivalent
}

// Draws the stage title and borders
void Stage::drawStageScreen()
{
    Point currentDrawPoint(Game::StartScreenPoint); // Top-left start point
    Point endDrawPoint(Game::EndScreenPoint);      // Bottom-right end point

    // Adjust stage boundaries
    currentDrawPoint += {6, 1};
    endDrawPoint -= {6, 3};

    // Draw stage title
    Game::setCursorPosition(currentDrawPoint);
    cout << "CONSOLE SNAKE v1.0";

    // Set text colors
    Game::setTextColors(BackgroundColor, ForegroundColor);

    // Draw top border
    currentDrawPoint += {0, 1};
    Game::setCursorPosition(currentDrawPoint);
    short setW = endDrawPoint.X() - currentDrawPoint.X();
    cout << '+' << setw(setW) << setfill('-') << '+';

    // Draw side borders
    currentDrawPoint += {0, 1};
    while (currentDrawPoint.Y() < endDrawPoint.Y())
    {
        Game::setCursorPosition(currentDrawPoint);
        cout << BorderCharacter << setw(setW) << setfill(' ') << BorderCharacter;
        currentDrawPoint += {0, 1};
    }

    // Draw bottom border
    Game::setCursorPosition(currentDrawPoint);
    cout << '+' << setw(setW) << setfill('-') << '+';
}