#include "../Headers/GridMap.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "../Headers/Game.h"
#include "../Headers/Stage.h"

using namespace std;

// Initialize static boundary variables (defaults)
Point GridMap::startMovePosition = {10, 6};
Point GridMap::endMovePosition = {100, 28};
Point GridMap::upperPortalPosition = {0, 0};
Point GridMap::lowerPortalPosition = {0, 0};
const ConsoleColor GridMap::BorderColor = ConsoleColor::White;

// Constructor
GridMap::GridMap(UI& ref) : ui(ref) {
    // Create a new Snake object on the heap
    snake = new Snake(ui);
    // Seed the random number generator for random food positions
    srand(static_cast<unsigned int>(time(nullptr))); 
}

// Destructor
GridMap::~GridMap() {
    // Free the memory used by the snake to prevent leaks
    if (snake) delete snake;
}

// Draws the static walls of the arena
void GridMap::drawGrid() {
    // Set color for the borders
    Game::setTextColors(BorderColor, Stage::BackgroundColor);

    // Draw Top and Bottom horizontal lines
    for (int x = startMovePosition.X(); x <= endMovePosition.X(); ++x) {
        Game::setCursorPosition(x, startMovePosition.Y());
        cout << "-";
        Game::setCursorPosition(x, endMovePosition.Y());
        cout << "-";
    }

    // Draw Left and Right vertical lines
    for (int y = startMovePosition.Y(); y <= endMovePosition.Y(); ++y) {
        Game::setCursorPosition(startMovePosition.X(), y);
        cout << "|";
        Game::setCursorPosition(endMovePosition.X(), y);
        cout << "|";
    }

    drawPortals(); // Draw the magical portals
}

// Calculates and draws the portals (teleporters)
void GridMap::drawPortals() {
    // Calculate middle X coordinate
    int midX = (startMovePosition.X() + endMovePosition.X()) / 2;
    
    // Place portals at the top and bottom center
    upperPortalPosition = { midX, startMovePosition.Y() };
    lowerPortalPosition = { midX, endMovePosition.Y() };
    
    // Draw them
    Game::setCursorPosition(upperPortalPosition);
    cout << "O"; 
    Game::setCursorPosition(lowerPortalPosition);
    cout << "O"; 
}

// The main loop for the ACTUAL gameplay (while playing)
int GridMap::run() {
    bool gameOver = false;

    // Ensure snake knows the map size
    snake->setupMovementBoundaries();

    // GAME LOOP: Keep running until game over
    while (!gameOver) {
        // processesGameplay() returns false if snake dies
        // The ! negates it, so gameOver becomes true if snake dies
        gameOver = !snake->processesGameplay(); 
        
        // Small delay to prevent CPU usage spike (though Snake controls speed too)
        this_thread::sleep_for(chrono::milliseconds(10)); 
    }

    // If we break the loop, we died
    showGameOver();
    
    // Pause for 2 seconds so player sees "Game Over"
    this_thread::sleep_for(chrono::seconds(2));
    
    return BACKTOSTART; // Tell Game.cpp to go back to Title Screen
}

// Display the Game Over text
void GridMap::showGameOver() {
    Game::setTextColors(ConsoleColor::Red, Stage::BackgroundColor);
    // Calculate center of screen to print text
    Game::setCursorPosition((startMovePosition.X() + endMovePosition.X()) / 2 - 5,
                            (startMovePosition.Y() + endMovePosition.Y()) / 2);
    cout << "GAME OVER!";
}