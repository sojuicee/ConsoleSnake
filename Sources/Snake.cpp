#include "../Headers/Snake.h"
#include "../Headers/Timer.h"
#include "../Headers/Game.h"
#include <iostream>
#include <algorithm>
#include <thread>
#include <chrono>

using namespace std;

// Constructor: Initializes the snake when the game starts
Snake::Snake(UI& ref) : 
    refUi(ref),                         // Save the reference to the UI (for score/speed)
    movementDirection(MoveRight),       // Default move direction is RIGHT
    lastMovementDirection(MoveRight),   // Track last move to prevent illegal 180 turns
    isGameOver(false)                   // Game is running, not over
{
    // Get the boundaries of the map so we know where walls are
    startMovePosition = GridMap::getStartMovePosition();
    endMovePosition = GridMap::getEndMovePosition();
    
    createSnakeHead();   // Initialize the head position
    createSnakeBody();   // Initialize the starting body segments
}

// Re-fetch boundaries (useful if stage size changes)
void Snake::setupMovementBoundaries() {
    startMovePosition = GridMap::getStartMovePosition();
    endMovePosition = GridMap::getEndMovePosition();
}

// Reads keyboard input to change direction
int Snake::processesInputs() {
    // Mac/Linux safe input check (using our helper or standard cin)
    // If no input is waiting, return 0 (do nothing)
    // Note: In the Mac version, this logic is handled by the Game/TitleScreen loop usually
    return 1; 
}

// Sets the initial position of the head
void Snake::createSnakeHead() {
    // Start exactly in the center of the arena
    Point start = GridMap::getCenterMovePosition();
    // Create the head BodyPiece at 'start' with symbol 'O'
    head = BodyPiece(start, 'O'); 
}

// Creates the initial tail/body behind the head
void Snake::createSnakeBody() {
    body.clear(); // Clear any existing body parts (for resets)
    Point pos = head.getPosition(); // Start at the head's position

    // Loop to create 'BodyInitialAmount' (e.g., 3) segments
    for (unsigned char i = 0; i < BodyInitialAmount; ++i) {
        pos += Point(-1, 0); // Move 1 step LEFT for the next segment
        // Add this new segment to the BACK of our list
        body.push_back(BodyPiece(pos, 'o')); 
    }
    // The last element in the list is the tail
    tail = body.back();
}

// THE MOST IMPORTANT FUNCTION: MOVES THE SNAKE
void Snake::movesTheSnake() {
    Point prevPos = head.getPosition(); // Remember where the head was

    // Move the head one step in the current direction
    head.setPosition(prevPos + movementDirection); 

    // Flags to store collision results
    bool collided = false, throughPortal = false, ateFood = false;
    
    // Check if this new move caused a crash or eating food
    checkCollisions(prevPos, collided, throughPortal, ateFood);

    // If we hit a wall or ourselves, set the game over flag
    if (collided) isGameOver = true;

    // LOGIC: Move the body to follow the head
    // We save the position of the previous segment to move the current one into it
    Point lastPos = prevPos;
    
    // Loop through every segment in the Linked List
    for (auto& segment : body) {
        Point temp = segment.getPosition(); // Save this segment's current pos
        segment.setPosition(lastPos);       // Move it to where the previous one was
        lastPos = temp;                     // Update lastPos for the next segment
    }
    
    // Update the tail pointer to the very last element
    tail = body.back();
    
    // Save direction to prevent 180-degree turns next frame
    lastMovementDirection = movementDirection;
}

// Handles all collision logic (Walls, Self, Food)
void Snake::checkCollisions(Point& prevHeadPos, bool& collided, bool& throughPortal, bool& ateFood) {
    Point headPos = head.getPosition(); // Get current head coordinates

    // 1. WALL COLLISION CHECK
    // If X or Y is outside the start/end boundaries...
    if (headPos.X() <= startMovePosition.X() || headPos.X() >= endMovePosition.X() ||
        headPos.Y() <= startMovePosition.Y() || headPos.Y() >= endMovePosition.Y()) {
        collided = true; // Crash!
        return;
    }

    // 2. SELF COLLISION CHECK
    // Loop through every body part
    for (auto& segment : body) {
        // If the head is in the exact same spot as a body part...
        if (headPos == segment.getPosition()) {
            collided = true; // Crash!
            return;
        }
    }

    // 3. PORTAL CHECK (Optional feature)
    if (GridMap::isPortalsEntrance(headPos)) {
        // If hitting top portal, teleport to bottom (and vice versa)
        if (headPos == GridMap::getUpperPortalPosition())
            head.setPosition(GridMap::getLowerPortalPosition());
        else
            head.setPosition(GridMap::getUpperPortalPosition());
    }

    // 4. FOOD CHECK
    // Loop through all food items on the map
    for (auto& food : Food::foodsOnGridMap) {
        // If head touches food...
        if (headPos == food->GridPosition) { // Note: Accessed via friend or public
            ateFood = true; // Mark as eaten
            refUi.addScorePoints(Food::PointsPerFood); // Increase score
            // Note: Growing logic usually involves adding a segment here
        }
    }
}

// The main loop step for the snake object
bool Snake::processesGameplay() {
    processesInputs(); // 1. Read keys
    movesTheSnake();   // 2. Move snake & check collisions
    
    // 3. Wait a tiny bit (controls game speed based on UI level)
    this_thread::sleep_for(chrono::milliseconds(refUi.getNextSpeedPanelValue()));
    
    return !isGameOver; // Return false if we died (ends game)
}