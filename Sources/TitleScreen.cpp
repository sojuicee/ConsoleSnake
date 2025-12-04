#include "TitleScreen.h"
#include "Game.h"
#include "Timer.h"
#include "Stage.h"
#include <iostream>
#include <cstdlib>  // For rand()
#include <conio.h>  // For _kbhit() and _getch()

using namespace std;

// Constructor
TitleScreen::TitleScreen()
{
    showEnterMessage = false;
    showChoiceArrow = false;
    blinkMsgPosition = { 52, 21 };
    playerChoiceArrow = { 52, 23 };
    titleScreenBackColors = { 1,2,3,4,6,7,8,9,10,11,12,13,14,15,30,32,48,78,87,96,113,125,128,142,159,160,222,224,240 };
    numberLimit = static_cast<short>(titleScreenBackColors.size());
    lastRandom = 5; // Default color index
}

// Destructor
TitleScreen::~TitleScreen() {}

// Prepares the title screen with timers
int TitleScreen::prepareTitleScreen()
{
    drawTitleScreen();

    // Timers to blink messages and change colors
    Timer::setTimerAndCallback(125, this, &TitleScreen::blinkPressEnterMsg_callBack);
    Timer::setTimerAndCallback(125, this, &TitleScreen::blinkChoiceArrow_callBack);
    Timer::setTimerAndCallback(2000, this, &TitleScreen::changeTitleScreenColors_callBack);

    return 0; // SUCCESS
}

// Waits for player input to select Start or Exit
bool TitleScreen::waitingForPlayerChoice()
{
    short yPosUp = playerChoiceArrow.Y();
    short yPosDown = yPosUp + 1;
    short key = 0;
    bool startGameplay = true;

    while (key != 13) // 13 = Enter key
    {
        Timer::run(); // Update timers

        if (_kbhit()) // If key pressed
        {
            key = _getch();

            // If special key, read second byte
            if (key == 0x00 || key == 0xE0)
                key = _getch();

            switch (key)
            {
            case 'w':
            case 'W':
            case 72: // ArrowUp
                if (!startGameplay)
                {
                    playerChoiceArrow = { playerChoiceArrow.X(), yPosUp };
                    Game::setCursorPosition(playerChoiceArrow);
                    cout << ">>";
                    Game::setCursorPosition(playerChoiceArrow.X(), yPosDown);
                    cout << "  ";
                    startGameplay = true;
                }
                break;

            case 's':
            case 'S':
            case 80: // ArrowDown
                if (startGameplay)
                {
                    playerChoiceArrow = { playerChoiceArrow.X(), yPosDown };
                    Game::setCursorPosition(playerChoiceArrow);
                    cout << ">>";
                    Game::setCursorPosition(playerChoiceArrow.X(), yPosUp);
                    cout << "  ";
                    startGameplay = false;
                }
                break;

            default:
                break;
            }
        }
    }

    Timer::clearAll(); // Stop all blinking timers
    return startGameplay;
}

// Return last used color code
short TitleScreen::getHexaColorsCode()
{
    return titleScreenBackColors[lastRandom];
}

// Draws the title screen layout
void TitleScreen::drawTitleScreen()
{
    system("cls"); // Clear console
    setsTitleScreenColors();

    Point startPoint = Game::StartScreenPoint;
    Point endPoint = Game::EndScreenPoint;

    // Draw borders
    short setWidth = endPoint.X() - startPoint.X();
    Game::setCursorPosition(startPoint);
    cout << '+' << string(setWidth, '-') << '+';
    for (short y = startPoint.Y() + 1; y < endPoint.Y(); y++)
    {
        Game::setCursorPosition(startPoint.X(), y);
        cout << Stage::BorderCharacter;
        Game::setCursorPosition(endPoint.X(), y);
        cout << Stage::BorderCharacter;
    }
    Game::setCursorPosition(startPoint.X(), endPoint.Y());
    cout << '+' << string(setWidth, '-') << '+';

    // Draw title text
    const short textY = 8;
    const string SnakeText[] = {
        " SSSSSS  NN   NN    AAA    KK   KK  EEEEEEE ",
        "SS       NN   NN   AA AA   KK  KK   EE      ",
        "SS       NNN  NN  AA   AA  KK KK    EE      ",
        " SSSSS   NN N NN  AAAAAAA  KKK      EEEEEE  ",
        "     SS  NN  NNN  AA   AA  KK KK    EE      ",
        "     SS  NN   NN  AA   AA  KK  KK   EE      ",
        "SSSSSS   NN   NN  AA   AA  KK   KK  EEEEEEE "
    };

    Point drawPoint = { static_cast<short>((endPoint.X() - SnakeText[0].length()) / 2), textY };
    Game::setCursorPosition(drawPoint);
    cout << "Console";

    for (auto& line : SnakeText)
    {
        drawPoint += {0, 1};
        Game::setCursorPosition(drawPoint);
        cout << line;
    }

    // Draw player options
    drawPoint += {19, 8};
    Game::setCursorPosition(drawPoint);
    cout << "Start"; // Start
    drawPoint += {1, 1};
    Game::setCursorPosition(drawPoint);
    cout << "Exit";   // Exit

    // DRAW CREATOR INFO
    string creatorString = "© Josh Mabini, Data Structures and Algorithm Final Project";
    currentDrawPoint = { static_cast<short>((endDrawPoint.X() - creatorString.length()) * 0.5f), endDrawPoint.Y() - 1};
    Game::setCursorPosition(currentDrawPoint); // center horizontally, near bottom
    cout << creatorString;

}

// Blink "Press Enter" message
void TitleScreen::blinkPressEnterMsg()
{
    showEnterMessage = !showEnterMessage;
    Game::setCursorPosition(blinkMsgPosition);
    if (showEnterMessage)
        cout << "Press Enter!";
    else
        cout << "            ";
}

// Blink choice arrow
void TitleScreen::blinkChoiceArrow()
{
    showChoiceArrow = !showChoiceArrow;
    Game::setCursorPosition(playerChoiceArrow);
    if (showChoiceArrow)
        cout << ">>";
    else
        cout << "  ";
}

// Randomly change background color
void TitleScreen::changeTitleScreenColors()
{
    short randomNumber;
    do
    {
        randomNumber = rand() % numberLimit;
    } while (lastRandom == randomNumber);

    lastRandom = randomNumber;
    setsTitleScreenColors();
}

// Apply color to console
void TitleScreen::setsTitleScreenColors()
{
    short colorCode = titleScreenBackColors[lastRandom];
    #ifdef _WIN32
        // Windows console color
        system(("Color " + to_string(colorCode)).c_str());
    #else
        // Mac/Linux fallback: no color support by default
        // Could use ANSI codes if desired
    #endif
}

// Timer callback wrappers
void TitleScreen::blinkPressEnterMsg_callBack(void* ownerObject)
{
    reinterpret_cast<TitleScreen*>(ownerObject)->blinkPressEnterMsg();
}

void TitleScreen::blinkChoiceArrow_callBack(void* ownerObject)
{
    reinterpret_cast<TitleScreen*>(ownerObject)->blinkChoiceArrow();
}

void TitleScreen::changeTitleScreenColors_callBack(void* ownerObject)
{
    reinterpret_cast<TitleScreen*>(ownerObject)->changeTitleScreenColors();
}
