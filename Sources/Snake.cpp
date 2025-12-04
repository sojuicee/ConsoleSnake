#include "../Headers/Snake.h"
#include "../Headers/Timer.h"
#include "../Headers/Game.h" // Needed for ConsoleColor definition
#include "../Headers/Stage.h"
#include <iostream>
#include <algorithm>
#include <thread>
#include <chrono>

#include "../Headers/MacCompatibility.h" // For _kbhit/_getch

using namespace std;

// Initialize Colors in Constructor List
Snake::Snake(UI& ref) : 
    refUi(ref), 
    MoveUp({ 0, -1 }), MoveDown({ 0, 1 }), MoveLeft({ -2, 0 }), MoveRight({ 2, 0 }), 
    BodyInitialAmount(3),
    BackgroundColor(ConsoleColor::Black), 
    HeadForegroundColor(ConsoleColor::BrightCyan), 
    CollisionForegroundColor(ConsoleColor::Red),
    BodyForegroundColor(ConsoleColor::Green), 
    TailForegroundColor(ConsoleColor::Green),
    movementDirection(MoveRight), 
    lastMovementDirection(MoveRight), 
    isGameOver(false)
{
    startMovePosition = GridMap::getStartMovePosition();
    endMovePosition = GridMap::getEndMovePosition();
    createSnakeHead();
    createSnakeBody();
}

void Snake::setupMovementBoundaries() {
    startMovePosition = GridMap::getStartMovePosition();
    endMovePosition = GridMap::getEndMovePosition();
}

int Snake::processesInputs() {
    if (_kbhit()) {
        int key = _getch();
        // Handle Arrow Keys on Mac (Sequence: 27, 91, X)
        if (key == 27) { 
            _getch(); // Skip [
            key = _getch();
            switch(key) {
                case 65: movementDirection = MoveUp; break;
                case 66: movementDirection = MoveDown; break;
                case 68: movementDirection = MoveLeft; break;
                case 67: movementDirection = MoveRight; break;
            }
        }
        // Handle WASD
        else {
            switch (key) {
                case 'w': case 'W': movementDirection = MoveUp; break;
                case 's': case 'S': movementDirection = MoveDown; break;
                case 'a': case 'A': movementDirection = MoveLeft; break;
                case 'd': case 'D': movementDirection = MoveRight; break;
            }
        }
    }
    return 1;
}

void Snake::createSnakeHead() {
    Point start = GridMap::getCenterMovePosition();
    head = BodyPiece(start, '@'); // @ for head
}

void Snake::createSnakeBody() {
    body.clear();
    Point pos = head.getPosition();
    for (unsigned char i = 0; i < BodyInitialAmount; ++i) {
        pos += Point(-1, 0); 
        body.push_back(BodyPiece(pos, 'o'));
    }
    tail = body.back();
}

void Snake::movesTheSnake() {
    Point prevPos = head.getPosition();
    head.setPosition(prevPos + movementDirection); 

    bool collided = false, throughPortal = false, ateFood = false;
    checkCollisions(prevPos, collided, throughPortal, ateFood);

    if (collided) isGameOver = true;

    Point lastPos = prevPos;
    for (auto& segment : body) {
        Point temp = segment.getPosition();
        segment.setPosition(lastPos);
        lastPos = temp;
    }
    tail = body.back();
    lastMovementDirection = movementDirection;
    
    printTheBodyWhenMoving(prevPos, collided, false, ateFood);
}

void Snake::checkCollisions(Point& prevHeadPos, bool& collided, bool& throughPortal, bool& ateFood) {
    Point headPos = head.getPosition();

    // Wall Collision
    if (headPos.X() <= startMovePosition.X() || headPos.X() >= endMovePosition.X() ||
        headPos.Y() <= startMovePosition.Y() || headPos.Y() >= endMovePosition.Y()) {
        collided = true;
    }

    // Self Collision
    for (auto& segment : body) {
        if (headPos == segment.getPosition()) collided = true;
    }

    // Food Collision
    for (auto& food : Food::foodsOnGridMap) { // Note: accessing public static vector
        // Assuming Food class needs adjustment to expose position, 
        // OR we just assume collision for presentation demo purposes
    }
}

void Snake::printTheBodyWhenMoving(const Point& refLastPosition, const bool& refGridMapCollisionFlag,
    const bool& refBodyCollisionFlag, const bool& refGotFoodFlag) {
    
    if (!isGameOver) {
        Game::setTextColors(BackgroundColor, HeadForegroundColor);
        head.printBodyPiece();
    }
    
    if (!body.empty()) {
        Game::setTextColors(BackgroundColor, BodyForegroundColor);
        body.front().printBodyPiece();
    }

    // Erase old tail
    if (!refGotFoodFlag) {
        Game::setCursorPosition(refLastPosition);
        cout << ' ';
    }
}

bool Snake::processesGameplay() {
    processesInputs();
    movesTheSnake();
    return !isGameOver;
}

void Snake::movesTheSnake_callBack(void* ownerObject) {}
void Snake::processesInputs_callBack(void* ownerObject) {}