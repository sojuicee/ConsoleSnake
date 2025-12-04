#include "Game.h"
#include <iostream>
#include <thread>   // For sleep
#include <chrono>
#include <cstdlib>

#ifdef _WIN32
#include <conio.h>  // For _kbhit and _getch on Windows
#else
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#endif

using namespace std;

// Initialize static members
unsigned short Game::lastHiScorePoints = 0;
const Point Game::StartScreenPoint = {0, 0};
const Point Game::EndScreenPoint = {CONSOLEWIDTH, CONSOLEHEIGHT};

// Constructor
Game::Game() {
    ptrStage = nullptr;
    gameStates = GameStates::TitleScreen;
    setupConsoleWindow();
}

// Destructor
Game::~Game() {
    if (ptrStage) delete ptrStage;
    prepareToCloseWindow();
}

// Run the main game loop
int Game::run() {
    while (gameStates != GameStates::Quit) {
        switch (gameStates) {
            case GameStates::TitleScreen:
                ptrTitleScreen.prepareTitleScreen();
                gameStates = ptrTitleScreen.waitingForPlayerChoice() ? GameStates::GamePlay : GameStates::Quit;
                break;

            case GameStates::GamePlay:
                if (!ptrStage) ptrStage = new Stage();
                gameStates = (ptrStage->run() == BACKTOSTART) ? GameStates::TitleScreen : GameStates::Quit;
                break;

            default:
                gameStates = GameStates::Quit;
                break;
        }
    }
    return SUCCESS;
}

// Cross-platform cursor positioning
void Game::setCursorPosition(int x, int y) {
#ifdef _WIN32
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
#else
    cout << "\033[" << y + 1 << ";" << x + 1 << "H"; // ANSI escape for macOS/Linux
#endif
}

void Game::setCursorPosition(const Point& p) {
    setCursorPosition(p.X(), p.Y());
}

// Get character at position (simplified, placeholder)
char Game::getCursorPositionData(const Point&) {
    return ' '; // Full implementation requires platform-specific code
}

// Cross-platform text colors (basic)
void Game::setTextColors(ConsoleColor foreground, ConsoleColor background) {
#ifdef _WIN32
    // Windows console colors
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (WORD)(((int)background << 4) | (int)foreground));
#else
    // ANSI escape codes for macOS/Linux
    int fg = (int)foreground;
    int bg = (int)background;
    cout << "\033[" << 30 + fg << ";" << 40 + bg << "m";
#endif
}

// Reset colors to default
void Game::backToTitleScreenColors() {
#ifdef _WIN32
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
#else
    cout << "\033[0m";
#endif
}

// Setup console window (optional, minimal for macOS/Linux)
void Game::setupConsoleWindow() {
    // On Windows: hide cursor or set size can be added
    // On macOS/Linux: minimal required
}

// Prepare to close console (optional)
void Game::prepareToCloseWindow() {
#ifdef _WIN32
    // Reset color before exit
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
#else
    cout << "\033[0m"; // Reset ANSI colors
#endif
}