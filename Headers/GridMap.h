#pragma once

#include "Snake.h"
#include "Point.h"
#include "UI.h"
#include "Food.h"

// Enum for text colors
enum class ConsoleColor;

// Class to manage the game arena/grid
class GridMap
{
private:
    Snake* snake;               // The snake object
    UI& ui;                     // Reference to UI for panel updates

    // Arena boundaries and portals
    static Point startMovePosition;
    static Point endMovePosition;
    static Point upperPortalPosition;
    static Point lowerPortalPosition;

    void drawPortals();         // Draw portals
    void showGameOver();        // Show "Game Over" screen

public:
    static const ConsoleColor BorderColor;

    GridMap(UI& ref);           // Constructor
    ~GridMap();                 // Destructor

    void drawGrid();            // Draw arena
    int run();                  // Run gameplay loop

    // Helper functions for portals and positions
    static bool isPortalsEntrance(const Point& pointToCheck);
    static Point getUpperPortalPosition();
    static Point getLowerPortalPosition();
    static Point getStartMovePosition();
    static Point getEndMovePosition();
    static Point getCenterMovePosition();
    static Point getRandomPosition();
};