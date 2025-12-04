#include "../Headers/UI.h"
#include "../Headers/Stage.h"
#include "../Headers/Timer.h"
#include "../Headers/Game.h"
#include <iostream>
#include <iomanip>

using namespace std;

const vector<ushort> UI::SnakeSpeedTimes = {38, 39, 40, 43, 46, 51, 56, 63, 70, 80, 90, 110, 130, 160, 190, 225, 260};

UI::UI() :
    BackgroundColor(Stage::ForegroundColor),
    // CHANGED COLORS HERE FOR MAC
    ScoreForegroundColor(ConsoleColor::BrightGreen),
    HiScoreForegroundColor(ConsoleColor::BrightCyan),
    SpeedForegroundColor(ConsoleColor::Yellow),
    PointsToIncreaseSpeed(300)
{
    showScorePoints = true;
    showHiScorePoints = true;
    showSpeedValue = true;

    scorePanelPosition = { 9, 3 };
    hiScorePanelPosition = { 32, 3 };
    snakeSpeedPanelPosition = { 56, 3 };

    scorePanelPoints = 0;
    nextScoreValue = 0;
    // This now works because we made lastHiScorePoints public in Game.h
    hiScorePanelPoints = Game::lastHiScorePoints;
    speedTimeIndex = static_cast<ushort>(SnakeSpeedTimes.size()) - 1;
    nextSnakeSpeedTime = SnakeSpeedTimes[speedTimeIndex];
    snakeSpeedPanelValue = nextSnakeSpeedTime;
}

void UI::setupUI() { drawUI(); }

void UI::addScorePoints(ushort pointsToAdd)
{
    deleteUItimers();
    nextScoreValue += pointsToAdd;
    blinkMethods.push_back(&UI::blinkScore_callBack);

    if (hiScorePanelPoints < nextScoreValue) {
        blinkMethods.push_back(&UI::blinkHiScore_callBack);
        Game::lastHiScorePoints = nextScoreValue;
    }

    if (nextSnakeSpeedTime > SnakeSpeedTimes[0] && (nextScoreValue % PointsToIncreaseSpeed) == 0) {
        nextSnakeSpeedTime = SnakeSpeedTimes[--speedTimeIndex];
        blinkMethods.push_back(&UI::blinkSpeedValue_callBack);
    }

    Timer::setTimerAndCallback(1000, this, &UI::prepareToStopBlinking_callBack);
    for (auto method : blinkMethods) Timer::setTimerAndCallback(100, this, method);
}

void UI::deleteUItimers() {
    Timer::markTimerForDeletion(this, &UI::prepareToStopBlinking_callBack);
    Timer::markTimerForDeletion(this, &UI::stopBlinking_callBack);
    for (auto method : blinkMethods) Timer::markTimerForDeletion(this, method);
    blinkMethods.clear();
    writeAllPanelValues();
}

void UI::drawUI() {
    Game::setCursorPosition(scorePanelPosition);
    cout << " SCORE: ";
    writeAllPanelValues();
}

void UI::blinkPanelValue(bool& showPanelValue, const ConsoleColor& valueForegroundColor) {
    showPanelValue = !showPanelValue;
    if (showPanelValue) Game::setTextColors(BackgroundColor, valueForegroundColor);
    else Game::setTextColors(BackgroundColor, BackgroundColor);
}

void UI::writePanelValue(const Point& panelPosition, const uchar setW, const ushort& panelValue) {
    Game::setCursorPosition(panelPosition);
    cout << setfill('0') << setw(setW) << panelValue;
}

void UI::writeAllPanelValues() {
    Game::setTextColors(BackgroundColor, ScoreForegroundColor);
    writePanelValue(scorePanelPosition, 7, scorePanelPoints);
    Game::setTextColors(BackgroundColor, HiScoreForegroundColor);
    writePanelValue(hiScorePanelPosition, 7, hiScorePanelPoints);
    Game::setTextColors(BackgroundColor, SpeedForegroundColor);
    writePanelValue(snakeSpeedPanelPosition, 3, snakeSpeedPanelValue);
}

void UI::prepareToStopBlinking() {
    scorePanelPoints = nextScoreValue;
    if (hiScorePanelPoints < scorePanelPoints) hiScorePanelPoints = scorePanelPoints;
    snakeSpeedPanelValue = nextSnakeSpeedTime;
    Timer::setTimerAndCallback(1000, this, &UI::stopBlinking_callBack);
    for (auto method : blinkMethods) Timer::setTimerAndCallback(200, this, method);
    Timer::markTimerForDeletion(this, &UI::prepareToStopBlinking_callBack);
}

void UI::stopBlinking() {
    for (auto method : blinkMethods) Timer::markTimerForDeletion(this, method);
    Timer::markTimerForDeletion(this, &UI::stopBlinking_callBack);
    showScorePoints = true; showHiScorePoints = true; showSpeedValue = true;
    writeAllPanelValues();
    blinkMethods.clear();
}

void UI::blinkScore_callBack(void* ownerObject) {
    UI* ptrUi = reinterpret_cast<UI*>(ownerObject);
    ptrUi->blinkPanelValue(ptrUi->showScorePoints, ptrUi->ScoreForegroundColor);
    ptrUi->writePanelValue(ptrUi->scorePanelPosition, 7, ptrUi->scorePanelPoints);
}
void UI::blinkHiScore_callBack(void* ownerObject) {
    UI* ptrUi = reinterpret_cast<UI*>(ownerObject);
    ptrUi->blinkPanelValue(ptrUi->showHiScorePoints, ptrUi->HiScoreForegroundColor);
    ptrUi->writePanelValue(ptrUi->hiScorePanelPosition, 7, ptrUi->hiScorePanelPoints);
}
void UI::blinkSpeedValue_callBack(void* ownerObject) {
    UI* ptrUi = reinterpret_cast<UI*>(ownerObject);
    ptrUi->blinkPanelValue(ptrUi->showSpeedValue, ptrUi->SpeedForegroundColor);
    ptrUi->writePanelValue(ptrUi->snakeSpeedPanelPosition, 3, ptrUi->snakeSpeedPanelValue);
}
void UI::prepareToStopBlinking_callBack(void* ownerObject) { reinterpret_cast<UI*>(ownerObject)->prepareToStopBlinking(); }
void UI::stopBlinking_callBack(void* ownerObject) { reinterpret_cast<UI*>(ownerObject)->stopBlinking(); }