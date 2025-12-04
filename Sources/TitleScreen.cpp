#include "../Headers/TitleScreen.h"
#include "../Headers/Game.h"
#include "../Headers/Timer.h"
#include "../Headers/Stage.h"
#include <iostream>
#include <cstdlib>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

using namespace std;

// Helper function to check for key presses on Mac
int mac_kbhit() {
    struct termios oldt, newt;
    int ch;
    int oldf;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    if(ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}

TitleScreen::TitleScreen()
{
    showEnterMessage = false;
    showChoiceArrow = false;
    blinkMsgPosition = { 52, 21 };
    playerChoiceArrow = { 52, 23 };
    // Simplified colors for Mac
    titleScreenBackColors = { 1, 2, 3, 4, 5, 6 };
    numberLimit = (short)titleScreenBackColors.size();
    lastRandom = 0;
}

TitleScreen::~TitleScreen() {}

int TitleScreen::prepareTitleScreen()
{
    drawTitleScreen();
    Timer::setTimerAndCallback(125, this, &TitleScreen::blinkPressEnterMsg_callBack);
    Timer::setTimerAndCallback(125, this, &TitleScreen::blinkChoiceArrow_callBack);
    return 0;
}

bool TitleScreen::waitingForPlayerChoice()
{
    short yPosUp = playerChoiceArrow.Y();
    short yPosDown = yPosUp + 1;
    char key = 0;
    bool startGameplay = true;

    while (true)
    {
        Timer::run();

        if (mac_kbhit())
        {
            key = getchar();
            if (key == '\n') break; // Enter key

            if (key == 'w' || key == 'W') {
                if (!startGameplay) {
                    playerChoiceArrow = { playerChoiceArrow.X(), yPosUp };
                    Game::setCursorPosition(playerChoiceArrow);
                    cout << ">>";
                    Game::setCursorPosition(playerChoiceArrow.X(), yPosDown);
                    cout << "  ";
                    startGameplay = true;
                }
            }
            else if (key == 's' || key == 'S') {
                if (startGameplay) {
                    playerChoiceArrow = { playerChoiceArrow.X(), yPosDown };
                    Game::setCursorPosition(playerChoiceArrow);
                    cout << ">>";
                    Game::setCursorPosition(playerChoiceArrow.X(), yPosUp);
                    cout << "  ";
                    startGameplay = false;
                }
            }
        }
    }

    Timer::clearAll();
    return startGameplay;
}

short TitleScreen::getHexaColorsCode() { return 0; }

void TitleScreen::drawTitleScreen()
{
    system("clear");
    Point startPoint = Game::StartScreenPoint;
    Point endPoint = Game::EndScreenPoint;

    // Draw Title Text manually
    Game::setCursorPosition(50, 10);
    cout << "CONSOLE SNAKE";
    
    Game::setCursorPosition(52, 23);
    cout << "Start Game";
    Game::setCursorPosition(52, 24);
    cout << "Exit";
}

void TitleScreen::blinkPressEnterMsg() {
    showEnterMessage = !showEnterMessage;
    Game::setCursorPosition(blinkMsgPosition);
    cout << (showEnterMessage ? "Press Enter!" : "            ");
}

void TitleScreen::blinkChoiceArrow() {
    showChoiceArrow = !showChoiceArrow;
    Game::setCursorPosition(playerChoiceArrow);
    cout << (showChoiceArrow ? ">>" : "  ");
}

void TitleScreen::changeTitleScreenColors() {}
void TitleScreen::setsTitleScreenColors() {}

void TitleScreen::blinkPressEnterMsg_callBack(void* ownerObject) {
    reinterpret_cast<TitleScreen*>(ownerObject)->blinkPressEnterMsg();
}

void TitleScreen::blinkChoiceArrow_callBack(void* ownerObject) {
    reinterpret_cast<TitleScreen*>(ownerObject)->blinkChoiceArrow();
}

void TitleScreen::changeTitleScreenColors_callBack(void* ownerObject) {}