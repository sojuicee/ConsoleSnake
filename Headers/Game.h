#pragma once

#include <vector>
#include "TitleScreen.h"
#include "Stage.h"
#include "Point.h"
#include "UI.h"

// Mac-Compatible Colors
enum class ConsoleColor { Black, Red, Green, Yellow, Blue, Magenta, Cyan, White, BrightBlack, BrightRed, BrightGreen, BrightYellow, BrightBlue, BrightMagenta, BrightCyan, BrightWhite };

enum class GameStates { TitleScreen, WaitOption, GamePlay, Quit };

enum class KeyValues {
    Enter       = 0x0D,
    ArrowUp     = 0x48,
    ArrowDown   = 0x50,
    ArrowLeft   = 0x4B,
    ArrowRight  = 0x4D
};

#define CONSOLEWIDTH    114
#define CONSOLEHEIGHT   33

#define GAMEERROR   -1
#define SUCCESS     0
#define BACKTOSTART 1

class Game
{
private:
    TitleScreen             ptrTitleScreen;
    Stage* ptrStage;
    GameStates              gameStates;

    void setupConsoleWindow();
    void prepareToCloseWindow();

public:
    // MOVED TO PUBLIC FOR UI ACCESS
    static unsigned short   lastHiScorePoints; 
    static const Point      StartScreenPoint;
    static const Point      EndScreenPoint;

    Game();
    ~Game();
    int run();
    
    static void setCursorPosition(int x, int y);
    static void setCursorPosition(const Point& cursorCoordinate);
    static char getCursorPositionData(const Point& cursorCoordinate);
    static void setTextColors(ConsoleColor backgroundColor, ConsoleColor foregroundColor);
    static void backToTitleScreenColors();
};