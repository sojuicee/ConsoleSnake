#pragma once

#include <vector>
#include "TitleScreen.h"
#include "Stage.h"
#include "Point.h"
#include "UI.h"

// Enum for text colors
enum class ConsoleColor { Black, Red, Green, Yellow, Blue, Magenta, Cyan, White, BrightBlack, BrightRed, BrightGreen, BrightYellow, BrightBlue, BrightMagenta, BrightCyan, BrightWhite };

// Enum for game states
enum class GameStates { TitleScreen, WaitOption, GamePlay, Quit };

// Enum for key values
enum class KeyValues {
    Enter       = 0x0D,
    ArrowUp     = 0x48,
    ArrowDown   = 0x50,
    ArrowLeft   = 0x4B,
    ArrowRight  = 0x4D
};

// Console dimensions
#define CONSOLEWIDTH    114
#define CONSOLEHEIGHT   33

#define GAMEERROR   -1
#define SUCCESS     0
#define BACKTOSTART 1

class Game
{
private:
    TitleScreen ptrTitleScreen; // Title screen object
    Stage* ptrStage;            // Stage object
    GameStates gameStates;      // Current game state

    static unsigned short lastHiScorePoints; // Global high score

    // Internal helper functions
    void setupConsoleWindow(); 
    void prepareToCloseWindow();

public:
    static const Point StartScreenPoint; // Top-left point of screen
    static const Point EndScreenPoint;   // Bottom-right point of screen

    Game();
    ~Game();

    int run(); // Run the game

    // Cross-platform cursor management
    static void setCursorPosition(int x, int y);
    static void setCursorPosition(const Point& cursorCoordinate);

    // Get character at position (for collision, etc.)
    static char getCursorPositionData(const Point& cursorCoordinate);

    // Cross-platform text color
    static void setTextColors(ConsoleColor foreground, ConsoleColor background);

    // Reset colors to default
    static void backToTitleScreenColors();
};
