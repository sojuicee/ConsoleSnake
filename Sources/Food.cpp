#include "Food.h"
#include "Game.h"
#include "Timer.h"
#include <iostream>
#include <cstdlib> // For rand()

using namespace std;

// Initialize static variables
vector<Food*> Food::foodsOnGridMap;
Point Food::startMovePosition;
Point Food::endMovePosition;
const char Food::FoodCharacter = '*';
const short Food::PointsPerFood = 50;
const ConsoleColor Food::ForegroundColor = ConsoleColor::LightRed;
const ConsoleColor Food::BackgroundColor = ConsoleColor::Gray;
const unsigned char Food::TriesAmount = 50;

// Constructor
Food::Food(Point& spawnPos, unsigned char lifeTimeInSecs)
    : GridPosition(spawnPos),
      RemainingTimeToStartBlinking(5), // Example default
      MinSqrDistanceToSnakeHead(2),
      lifeTimeInSeconds(lifeTimeInSecs),
      showFoodOnGridMap(true)
{
    foodsOnGridMap.push_back(this); // Add to global list
}

// Display the food character at its position
void Food::writeFoodCharacter()
{
    if (showFoodOnGridMap)
    {
        Game::setCursorPosition(GridPosition);
        cout << FoodCharacter;
    }
}

// Reduce life time of the food
void Food::decreaseLifeTime()
{
    if (--lifeTimeInSeconds <= 0)
    {
        // Remove food from grid
        deleteFoodOnGridMap(GridPosition);
    }
}

// Toggle blinking of the food
void Food::blinkFoodOnGridMap()
{
    showFoodOnGridMap = !showFoodOnGridMap;
    writeFoodCharacter();
}

// Timer callback wrappers
void Food::decreaseLifeTime_callBack(void* ownerObject)
{
    reinterpret_cast<Food*>(ownerObject)->decreaseLifeTime();
}

void Food::blinkFoodOnGridMap_callBack(void* ownerObject)
{
    reinterpret_cast<Food*>(ownerObject)->blinkFoodOnGridMap();
}

// Start automatic spawning (to be implemented as needed)
void Food::startFoodSpawner()
{
    Timer::setTimerAndCallback(1000, nullptr, runFoodSpawner);
}

// Stop spawning food
void Food::stopFoodSpawner()
{
    Timer::markTimerForDeletion(nullptr, runFoodSpawner);
}

// Delete a specific food from the map
void Food::deleteFoodOnGridMap(Point gridPosition)
{
    for (size_t i = foodsOnGridMap.size(); i > 0; i--)
    {
        if (foodsOnGridMap[i - 1]->GridPosition == gridPosition)
        {
            delete foodsOnGridMap[i - 1]; // Free memory
            foodsOnGridMap.erase(foodsOnGridMap.begin() + (i - 1));
        }
    }
}

// The actual spawner function (placeholder)
void Food::runFoodSpawner(void*)
{
    // Example: you could randomly generate positions and create Food objects
}
