#include "Snake.h"
#include "Timer.h"
#include "Game.h"
#include <iostream>
#include <algorithm>
#include <thread>
#include <chrono>

using namespace std;

// Constructor: initialize snake and direction
Snake::Snake(UI& ref) : refUi(ref), movementDirection(MoveRight), lastMovementDirection(MoveRight), isGameOver(false)
{
    startMovePosition = GridMap::getStartMovePosition();
    endMovePosition = GridMap::getEndMovePosition();
    createSnakeHead();   // Create the head at the start
    createSnakeBody();   // Create initial body pieces
}

// Set boundaries for movement from GridMap
void Snake::setupMovementBoundaries() {
    startMovePosition = GridMap::getStartMovePosition();
    endMovePosition = GridMap::getEndMovePosition();
}

// Handle player input, returns 1 if input processed
int Snake::processesInputs() {
    if (!cin.rdbuf()->in_avail()) return 0; // Non-blocking input check

    char inputChar;
    cin >> inputChar; // Read character from user
    switch (inputChar) {
        case 'w': case 'W': movementDirection = MoveUp; break;
        case 's': case 'S': movementDirection = MoveDown; break;
        case 'a': case 'A': movementDirection = MoveLeft; break;
        case 'd': case 'D': movementDirection = MoveRight; break;
        default: break;
    }

    // Prevent snake from reversing
    if ((movementDirection.X() + lastMovementDirection.X() == 0) &&
        (movementDirection.Y() + lastMovementDirection.Y() == 0))
        movementDirection = lastMovementDirection;

    return 1;
}

// Create the snake head at starting position
void Snake::createSnakeHead() {
    Point start = GridMap::getCenterMovePosition();
    head = BodyPiece(start, 'O'); // 'O' for head
}

// Create the initial body pieces behind head
void Snake::createSnakeBody() {
    body.clear();
    Point pos = head.getPosition();
    for (unsigned char i = 0; i < BodyInitialAmount; ++i) {
        pos += Point(-1, 0); // Place body to the left
        body.push_back(BodyPiece(pos, 'o')); // 'o' for body
    }
    tail = body.back();
}

// Move the snake each frame
void Snake::movesTheSnake() {
    Point prevPos = head.getPosition();

    head.setPosition(prevPos + movementDirection); // Move head

    // Check collisions
    bool collided = false, throughPortal = false, ateFood = false;
    checkCollisions(prevPos, collided, throughPortal, ateFood);

    if (collided) isGameOver = true;

    // Move body pieces
    Point lastPos = prevPos;
    for (auto& segment : body) {
        Point temp = segment.getPosition();
        segment.setPosition(lastPos);
        lastPos = temp;
    }
    tail = body.back();
    lastMovementDirection = movementDirection;
}

// Check collisions with wall, portals, self
void Snake::checkCollisions(Point& prevHeadPos, bool& collided, bool& throughPortal, bool& ateFood) {
    Point headPos = head.getPosition();

    // Collide with boundaries
    if (headPos.X() <= startMovePosition.X() || headPos.X() >= endMovePosition.X() ||
        headPos.Y() <= startMovePosition.Y() || headPos.Y() >= endMovePosition.Y()) {
        collided = true;
        return;
    }

    // Collide with self
    for (auto& segment : body) {
        if (headPos == segment.getPosition()) {
            collided = true;
            return;
        }
    }

    // Check portals
    if (GridMap::isPortalsEntrance(headPos)) {
        if (headPos == GridMap::getUpperPortalPosition())
            head.setPosition(GridMap::getLowerPortalPosition());
        else
            head.setPosition(GridMap::getUpperPortalPosition());
    }

    // Check food (simplified)
    for (auto& food : Food::foodsOnGridMap) {
        if (headPos == food->GridPosition) {
            ateFood = true;
            refUi.addScorePoints(Food::PointsPerFood);
        }
    }
}

// Main game loop process for Snake, returns false if game over
bool Snake::processesGameplay() {
    processesInputs();
    movesTheSnake();
    this_thread::sleep_for(chrono::milliseconds(refUi.getNextSpeedPanelValue()));
    return !isGameOver;
}

// Timer callbacks for cross-platform adaptation (if needed)
void Snake::movesTheSnake_callBack(void* ownerObject) {
    reinterpret_cast<Snake*>(ownerObject)->movesTheSnake();
}

void Snake::processesInputs_callBack(void* ownerObject) {
    reinterpret_cast<Snake*>(ownerObject)->processesInputs();
}
