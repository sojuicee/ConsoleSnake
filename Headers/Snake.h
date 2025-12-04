#pragma once

#include <list>
#include "Point.h"
#include "BodyPiece.h"
#include "UI.h"
#include "GridMap.h"

enum class ConsoleColor;

using std::list;

// Snake class manages all snake behavior
class Snake
{
private:
    UI& refUi;                     // Reference to UI for updating score/speed
    const Point MoveUp    = {0, -1};
    const Point MoveDown  = {0, 1};
    const Point MoveLeft  = {-1, 0};
    const Point MoveRight = {1, 0};

    const unsigned char BodyInitialAmount = 3;        // Initial body pieces
    const ConsoleColor BackgroundColor     = ConsoleColor::Gray;
    const ConsoleColor HeadForegroundColor = ConsoleColor::Purple;
    const ConsoleColor CollisionForegroundColor = ConsoleColor::Red;
    const ConsoleColor BodyForegroundColor = ConsoleColor::LightGreen;
    const ConsoleColor TailForegroundColor = ConsoleColor::Green;

    Point startMovePosition;       // Boundaries for movement
    Point endMovePosition;
    Point movementDirection;       // Current direction
    Point lastMovementDirection;   // Previous direction

    BodyPiece head;                // Snake head
    list<BodyPiece> body;          // Body segments
    BodyPiece tail;                // Tail segment

    bool isGameOver;               // Game over flag

    // Timer callbacks for movement and input
    static void movesTheSnake_callBack(void* ownerObject);
    static void processesInputs_callBack(void* ownerObject);

    // Internal functions
    int processesInputs();
    void createSnakeHead();
    void createSnakeBody();
    void movesTheSnake();
    void checkCollisions(Point&, bool&, bool&, bool&);
    void printTheBodyWhenMoving(const Point&, const bool&, const bool&, const bool&);

public:
    Snake(UI& ref);

    void setupMovementBoundaries();   // Sets grid movement limits
    bool processesGameplay();         // Returns false if collision/game over
};
