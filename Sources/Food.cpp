#include "../Headers/Food.h"
#include "../Headers/Game.h"
#include "../Headers/Timer.h"
#include <iostream>
#include <cstdlib> 

using namespace std;

// Static list to keep track of ALL food items currently on the map
vector<Food*> Food::foodsOnGridMap;

// Constants for food settings
const char Food::FoodCharacter = '*';
const short Food::PointsPerFood = 50;

// Constructor
Food::Food(Point& spawnPos, unsigned char lifeTimeInSecs)
    : GridPosition(spawnPos),       // Set position
      lifeTimeInSeconds(lifeTimeInSecs), // Set how long it lasts
      showFoodOnGridMap(true)       // It is visible
{
    // Important: Add *this* new food object to the global vector
    foodsOnGridMap.push_back(this); 
}

// Draws the food symbol
void Food::writeFoodCharacter() {
    if (showFoodOnGridMap) {
        Game::setCursorPosition(GridPosition); // Go to X,Y
        cout << FoodCharacter;                 // Print '*'
    }
}

// Removes a specific food item from the map and memory
void Food::deleteFoodOnGridMap(Point gridPosition) {
    // Loop backwards through the vector (safer when deleting)
    for (size_t i = foodsOnGridMap.size(); i > 0; i--) {
        // If we find the food at this position...
        if (foodsOnGridMap[i - 1]->GridPosition == gridPosition) {
            delete foodsOnGridMap[i - 1]; // Delete the object from HEAP memory
            // Remove the pointer from the vector
            foodsOnGridMap.erase(foodsOnGridMap.begin() + (i - 1));
        }
    }
}