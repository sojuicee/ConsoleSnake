#include "UI.h"
#include "Stage.h"
#include "Timer.h"
#include "Game.h"
#include <iostream>
#include <iomanip>

using namespace std; // Use standard namespace globally in this file

// Initialize static snake speed thresholds
const vector<ushort> UI::SnakeSpeedTimes = {38, 39, 40, 43, 46, 51, 56, 63, 70, 80, 90, 110, 130, 160, 190, 225, 260};

// Constructor: initialize UI panels and colors
UI::UI() :
    BackgroundColor(Stage::ForegroundColor),
    ScoreForegroundColor(ConsoleColor::LightGreen),
    HiScoreForegroundColor(ConsoleColor::LightAqua),
    SpeedForegroundColor(ConsoleColor::Yellow),
    PointsToIncreaseSpeed(300)
{
    // Enable all panel displays initially
    showScorePoints = true;
    showHiScorePoints = true;
    showSpeedValue = true;

    // Default positions (adjusted later)
    scorePanelPosition = { 9, 3 };
    hiScorePanelPosition = { 32, 3 };
    snakeSpeedPanelPosition = { 56, 3 };

    scorePanelPoints = 0;
    nextScoreValue = 0;
    hiScorePanelPoints = Game::lastHiScorePoints;
    speedTimeIndex = static_cast<ushort>(SnakeSpeedTimes.size()) - 1;
    nextSnakeSpeedTime = SnakeSpeedTimes[speedTimeIndex];
    snakeSpeedPanelValue = nextSnakeSpeedTime;
}

// Draw initial UI panels
void UI::setupUI()
{
    drawUI();
}

// Add points to score and handle blinking effects
void UI::addScorePoints(ushort pointsToAdd)
{
    deleteUItimers(); // Stop existing UI timers

    nextScoreValue += pointsToAdd;
    blinkMethods.push_back(&UI::blinkScore_callBack);

    // Update high score if exceeded
    if (hiScorePanelPoints < nextScoreValue)
    {
        blinkMethods.push_back(&UI::blinkHiScore_callBack);
        Game::lastHiScorePoints = nextScoreValue;
    }

    // Increase speed based on points
    if (nextSnakeSpeedTime > SnakeSpeedTimes[0] &&
        (nextScoreValue % PointsToIncreaseSpeed) == 0)
    {
        nextSnakeSpeedTime = SnakeSpeedTimes[--speedTimeIndex];
        if (nextSnakeSpeedTime == SnakeSpeedTimes[0])
            Timer::setTimerAndCallback(200, this, &UI::blinkSpeedValue_callBack);
        else
            blinkMethods.push_back(&UI::blinkSpeedValue_callBack);
    }

    // Prepare to stop blinking after 1 second
    Timer::setTimerAndCallback(1000, this, &UI::prepareToStopBlinking_callBack);

    // Start blinking timers
    for (auto method : blinkMethods)
        Timer::setTimerAndCallback(100, this, method);
}

// Get next speed panel value
ushort UI::getNextSpeedPanelValue() { return nextSnakeSpeedTime; }

// Get minimum snake speed
ushort UI::getMinimumTimeSpeed() { return SnakeSpeedTimes[0]; }

// Stop all blinking timers
void UI::deleteUItimers()
{
    Timer::markTimerForDeletion(this, &UI::prepareToStopBlinking_callBack);
    Timer::markTimerForDeletion(this, &UI::stopBlinking_callBack);

    for (auto method : blinkMethods)
        Timer::markTimerForDeletion(this, method);

    blinkMethods.clear();
    writeAllPanelValues(); // Redraw UI values
}

// Draw the UI panels and text
void UI::drawUI()
{
    Point currentDrawPoint(Game::StartScreenPoint); // top-left
    Point endDrawPoint(Game::EndScreenPoint);       // bottom-right

    // Adjust UI bounds
    currentDrawPoint += {6, 2};   // top-left inside margins
    endDrawPoint -= {6, 27};      // width and height margins

    // Set UI colors
    Game::setTextColors(BackgroundColor, ConsoleColor::BrightWhite);

    // Draw panel borders
    Game::setCursorPosition(hiScorePanelPosition.X(), hiScorePanelPosition.Y() + 1);
    cout << Stage::BorderCharacter;
    Game::setCursorPosition(snakeSpeedPanelPosition.X(), snakeSpeedPanelPosition.Y() + 1);
    cout << Stage::BorderCharacter;
    Game::setCursorPosition(endDrawPoint.X(), endDrawPoint.Y() - 1);
    cout << Stage::BorderCharacter;

    // Calculate panel widths
    short setWscoreToHiScore = hiScorePanelPosition.X() - scorePanelPosition.X();
    short setWhiScoreToSnakePanel = snakeSpeedPanelPosition.X() - hiScorePanelPosition.X();
    short setWsnakePanelToEndPanels = endDrawPoint.X() - snakeSpeedPanelPosition.X();

    // Draw top and bottom borders
    Game::setCursorPosition(currentDrawPoint);
    cout << '+' << setfill('-') << setw(setWscoreToHiScore) << '+'
         << setw(setWhiScoreToSnakePanel) << '+'
         << setw(setWsnakePanelToEndPanels) << '+';

    Game::setCursorPosition(currentDrawPoint.X(), endDrawPoint.Y());
    cout << '+' << setfill('-') << setw(setWscoreToHiScore) << '+'
         << setw(setWhiScoreToSnakePanel) << '+'
         << setw(setWsnakePanelToEndPanels) << '+';

    // Draw panel labels
    scorePanelPosition += {0, 1};
    hiScorePanelPosition += {0, 1};
    snakeSpeedPanelPosition += {0, 1};

    Game::setCursorPosition(scorePanelPosition);
    cout << Stage::BorderCharacter << " SCORE: ";
    Game::setCursorPosition(hiScorePanelPosition);
    cout << Stage::BorderCharacter << " HI-SCORE: ";
    Game::setCursorPosition(snakeSpeedPanelPosition);
    cout << Stage::BorderCharacter << " SPEED (ms): ";

    // Draw initial panel values
    scorePanelPosition += {10, 0};
    hiScorePanelPosition += {11, 0};
    snakeSpeedPanelPosition += {25, 0};

    Game::setTextColors(BackgroundColor, ScoreForegroundColor);
    writePanelValue(scorePanelPosition, 7, scorePanelPoints);
    cout << " pts  ";

    Game::setTextColors(BackgroundColor, HiScoreForegroundColor);
    writePanelValue(hiScorePanelPosition, 7, hiScorePanelPoints);
    cout << " pts  ";

    Game::setTextColors(BackgroundColor, SpeedForegroundColor);
    writePanelValue(snakeSpeedPanelPosition, 3, snakeSpeedPanelValue);
    cout << " ms       ";
}

// Blink a panel value for animation
void UI::blinkPanelValue(bool& showPanelValue, const ConsoleColor& valueForegroundColor)
{
    showPanelValue = !showPanelValue;
    if (showPanelValue)
        Game::setTextColors(BackgroundColor, valueForegroundColor);
    else
        Game::setTextColors(BackgroundColor, BackgroundColor);
}

// Write a numeric value in a panel
void UI::writePanelValue(const Point& panelPosition, const uchar setW, const ushort& panelValue)
{
    Game::setCursorPosition(panelPosition);
    cout << setfill('0') << setw(setW) << panelValue;
}

// Redraw all panel values
void UI::writeAllPanelValues()
{
    Game::setTextColors(BackgroundColor, ScoreForegroundColor);
    writePanelValue(scorePanelPosition, 7, scorePanelPoints);

    Game::setTextColors(BackgroundColor, HiScoreForegroundColor);
    writePanelValue(hiScorePanelPosition, 7, hiScorePanelPoints);

    Game::setTextColors(BackgroundColor, SpeedForegroundColor);
    writePanelValue(snakeSpeedPanelPosition, 3, snakeSpeedPanelValue);
}

// Prepare to stop blinking after timer
void UI::prepareToStopBlinking()
{
    scorePanelPoints = nextScoreValue;
    if (hiScorePanelPoints < scorePanelPoints)
        hiScorePanelPoints = scorePanelPoints;

    snakeSpeedPanelValue = nextSnakeSpeedTime;

    Timer::setTimerAndCallback(1000, this, &UI::stopBlinking_callBack);

    for (auto method : blinkMethods)
        Timer::setTimerAndCallback(200, this, method);

    Timer::markTimerForDeletion(this, &UI::prepareToStopBlinking_callBack);
}

// Stop all blinking effects
void UI::stopBlinking()
{
    for (auto method : blinkMethods)
        Timer::markTimerForDeletion(this, method);

    Timer::markTimerForDeletion(this, &UI::stopBlinking_callBack);

    showScorePoints = true;
    showHiScorePoints = true;
    showSpeedValue = true;
    writeAllPanelValues();

    blinkMethods.clear();
}

// Timer callback wrappers
void UI::blinkScore_callBack(void* ownerObject)
{
    UI* ptrUi = reinterpret_cast<UI*>(ownerObject);
    ptrUi->blinkPanelValue(ptrUi->showScorePoints, ptrUi->ScoreForegroundColor);
    ptrUi->writePanelValue(ptrUi->scorePanelPosition, 7, ptrUi->scorePanelPoints);
}

void UI::blinkHiScore_callBack(void* ownerObject)
{
    UI* ptrUi = reinterpret_cast<UI*>(ownerObject);
    ptrUi->blinkPanelValue(ptrUi->showHiScorePoints, ptrUi->HiScoreForegroundColor);
    ptrUi->writePanelValue(ptrUi->hiScorePanelPosition, 7, ptrUi->hiScorePanelPoints);
}

void UI::blinkSpeedValue_callBack(void* ownerObject)
{
    UI* ptrUi = reinterpret_cast<UI*>(ownerObject);
    ptrUi->blinkPanelValue(ptrUi->showSpeedValue, ptrUi->SpeedForegroundColor);
    ptrUi->writePanelValue(ptrUi->snakeSpeedPanelPosition, 3, ptrUi->snakeSpeedPanelValue);
}

void UI::prepareToStopBlinking_callBack(void* ownerObject)
{
    reinterpret_cast<UI*>(ownerObject)->prepareToStopBlinking();
}

void UI::stopBlinking_callBack(void* ownerObject)
{
    reinterpret_cast<UI*>(ownerObject)->stopBlinking();
}
