#include <iostream>
#include "BodyPiece.h"
#include "Game.h"   // Include Game class for console cursor positioning

using namespace std;

// Default constructor: sets position to (0,0) and symbol to 'O'
BodyPiece::BodyPiece() : gridPosition(0, 0), bodyConsoleSymbol('O') {}

// Constructor with custom position and symbol
BodyPiece::BodyPiece(Point gridPos, char bodySymbol)
    : gridPosition(gridPos), bodyConsoleSymbol(bodySymbol) {}

// Print the body piece at its current grid position
void BodyPiece::printBodyPiece()
{
    Game::setCursorPosition(gridPosition); // Move cursor to the piece position
    cout << bodyConsoleSymbol;             // Print the symbol
}

// Return the symbol of the body piece
char BodyPiece::getBodySymbol()
{
    return bodyConsoleSymbol;
}

// Return the position of the body piece
Point BodyPiece::getPosition()
{
    return gridPosition;
}

// Set a new position for the body piece
void BodyPiece::setPosition(Point newPosition)
{
    gridPosition = newPosition;
}

// Move the body piece relative to its current position
void BodyPiece::addToPosition(Point pointToAdd)
{
    gridPosition += pointToAdd; // Use Point's overloaded + operator
}