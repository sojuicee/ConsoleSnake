#pragma once

#include <vector>
#include "Point.h"

enum class ConsoleColor; // Forward declaration for colors

class Food
{
private:
    const Point GridPosition;                 // Where the food is on the grid
    const unsigned char RemainingTimeToStartBlinking; // Time before blinking starts
    const unsigned char MinSqrDistanceToSnakeHead;    // Minimum distance from snake
    static const ConsoleColor BackgroundColor;       // Background color of food
    static const unsigned char TriesAmount;          // Max attempts to spawn food
    unsigned char lifeTimeInSeconds;                 // How long food exists
    bool showFoodOnGridMap;                          // Whether to display food

    static std::vector<Food*> foodsOnGridMap;       // All foods on map
    static Point startMovePosition;                 // Grid start boundary
    static Point endMovePosition;                   // Grid end boundary

    // Internal callbacks for timers
    static void runFoodSpawner(void*);
    static void decreaseLifeTime_callBack(void* ownerObject);
    static void blinkFoodOnGridMap_callBack(void* ownerObject);

    // Internal methods
    void writeFoodCharacter();
    void decreaseLifeTime();
    void blinkFoodOnGridMap();

public:
    static const ConsoleColor ForegroundColor; // Color of the food
    static const char FoodCharacter;           // Character displayed
    static const short PointsPerFood;          // Points per food eaten

    Food(Point& spawnPos, unsigned char lifeTimeInSecs);

    static void startFoodSpawner();            // Start auto-spawning food
    static void stopFoodSpawner();             // Stop food spawner
    static void deleteFoodOnGridMap(Point gridPosition); // Remove specific food
};
