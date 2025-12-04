#include "GridMap.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Game.h"
#include "Stage.h"

using namespace std;

// Initialize static members
Point GridMap::startMovePosition = {10, 6};
Point GridMap::endMovePosition = {100, 28};
Point GridMap::upperPortalPosition = {0, 0};
Point GridMap::lowerPortalPosition = {0, 0};
const ConsoleColor GridMap::BorderColor = ConsoleColor::White;

// Constructor: initializes snake reference and UI
GridMap::GridMap(UI& ref) : ui(ref) {
    snake = new Snake(ui);
    srand(static_cast<unsigned int>(time(nullptr))); // For random positions
}

// Destructor: delete snake
GridMap::~GridMap() {
    if (snake) delete snake;
}

// Draw the arena boundaries
void GridMap::drawGrid() {
    Game::setTextColors(BorderColor, Stage::BackgroundColor);

    // Draw top and bottom borders
    for (int x = startMovePosition.X(); x <= endMovePosition.X(); ++x) {
        Game::setCursorPosition(x, startMovePosition.Y());
        cout << "-";
        Game::setCursorPosition(x, endMovePosition.Y());
        cout << "-";
    }

    // Draw left and right borders
    for (int y = startMovePosition.Y(); y <= endMovePosition.Y(); ++y) {
        Game::setCursorPosition(startMovePosition.X(), y);
        cout << "|";
        Game::setCursorPosition(endMovePosition.X(), y);
        cout << "|";
    }

    drawPortals();
}

// Draw portals at predefined positions
void GridMap::drawPortals() {
    upperPortalPosition = { (startMovePosition.X() + endMovePosition.X()) / 2, startMovePosition.Y() };
    lowerPortalPosition = { (startMovePosition.X() + endMovePosition.X()) / 2, endMovePosition.Y() };
    
    Game::setCursorPosition(upperPortalPosition);
    cout << "O"; // Portal symbol
    Game::setCursorPosition(lowerPortalPosition);
    cout << "O"; 
}

// Gameplay loop
int GridMap::run() {
    bool gameOver = false;

    snake->setupMovementBoundaries();

    while (!gameOver) {
        gameOver = !snake->processesGameplay(); // Returns false if collision occurs
        this_thread::sleep_for(chrono::milliseconds(50)); // Control snake speed
    }

    showGameOver();
    this_thread::sleep_for(chrono::seconds(2));
    return BACKTOSTART;
}

// Show game over screen
void GridMap::showGameOver() {
    Game::setTextColors(ConsoleColor::Red, Stage::BackgroundColor);
    Game::setCursorPosition((startMovePosition.X() + endMovePosition.X()) / 2 - 5,
                            (startMovePosition.Y() + endMovePosition.Y()) / 2);
    cout << "GAME OVER!";
}

// Static helpers
bool GridMap::isPortalsEntrance(const Point& pointToCheck) {
    return (pointToCheck == upperPortalPosition || pointToCheck == lowerPortalPosition);
}

Point GridMap::getUpperPortalPosition() { return upperPortalPosition; }
Point GridMap::getLowerPortalPosition() { return lowerPortalPosition; }
Point GridMap::getStartMovePosition() { return startMovePosition; }
Point GridMap::getEndMovePosition() { return endMovePosition; }
Point GridMap::getCenterMovePosition() {
    return { (startMovePosition.X() + endMovePosition.X()) / 2,
             (startMovePosition.Y() + endMovePosition.Y()) / 2 };
}
Point GridMap::getRandomPosition() {
    int x = startMovePosition.X() + rand() % (endMovePosition.X() - startMovePosition.X() - 1);
    int y = startMovePosition.Y() + rand() % (endMovePosition.Y() - startMovePosition.Y() - 1);
    return {x, y};
}