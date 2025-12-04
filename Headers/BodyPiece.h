#pragma once

#include "Point.h"   // Include the Point class for grid positions

using namespace std;

class BodyPiece
{
private:
    Point gridPosition;    // Position of this body piece on the grid
    char bodyConsoleSymbol; // Symbol to display in the console

public:
    BodyPiece();                                  // Default constructor
    BodyPiece(Point gridPos, char bodySymbol);   // Constructor with position and symbol

    void printBodyPiece();     // Prints the body piece at its position
    char getBodySymbol();      // Returns the symbol
    Point getPosition();       // Returns the current grid position
    void setPosition(Point newPosition); // Sets a new position
    void addToPosition(Point pointToAdd); // Moves the piece relative to current position
};