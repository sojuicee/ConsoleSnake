#pragma once

#include <list>
#include "Point.h"
#include "BodyPiece.h"
#include "UI.h"
#include "GridMap.h"

enum class ConsoleColor;

using std::list;

class Snake
{
private:
    UI&                     refUi;
    const Point             MoveUp;
    const Point             MoveDown;
    const Point             MoveLeft;
    const Point             MoveRight;
    const unsigned char     BodyInitialAmount;
    
    // Removed direct assignments to fix circular dependency
    const ConsoleColor      BackgroundColor;
    const ConsoleColor      HeadForegroundColor;
    const ConsoleColor      CollisionForegroundColor;
    const ConsoleColor      BodyForegroundColor;
    const ConsoleColor      TailForegroundColor;

    Point                   startMovePosition;
    Point                   endMovePosition;
    Point                   movementDirection;
    Point                   lastMovementDirection;
    BodyPiece               head;
    list<BodyPiece>         body;
    BodyPiece               tail;
    bool                    isGameOver;

    static void movesTheSnake_callBack(void* ownerObject);
    static void processesInputs_callBack(void* ownerObject);

    int processesInputs();
    void createSnakeHead();
    void createSnakeBody();
    void movesTheSnake();
    void checkCollisions(Point&, bool&, bool&, bool&);
    void printTheBodyWhenMoving(const Point&, const bool&, const bool&, const bool&);

public:
    Snake(UI& ref);
    void setupMovementBoundaries();
    bool processesGameplay();
};