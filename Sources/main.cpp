#include <iostream>
#include "../Headers/Game.h" // Include the main Game class

// Standard C++ Main Entry Point
int main()
{
    // Create an instance of the Game engine
    Game game;
    
    // Start the game loop and return its result (0 = success)
    return game.run();
}