#include "../Headers/UI.h"
#include "../Headers/Stage.h"
#include "../Headers/Timer.h"
#include "../Headers/Game.h"
#include <iostream>
#include <iomanip> // For setw (setting width of numbers)

using namespace std;

// Define the speed levels (lower number = faster snake)
const vector<ushort> UI::SnakeSpeedTimes = {38, 39, 40, 43, 46, 51, 56, 63, 70, 80, 90, 110, 130, 160, 190, 225, 260};

UI::UI() :
    BackgroundColor(Stage::ForegroundColor),
    ScoreForegroundColor(ConsoleColor::LightGreen),
    HiScoreForegroundColor(ConsoleColor::LightAqua),
    SpeedForegroundColor(ConsoleColor::Yellow),
    PointsToIncreaseSpeed(300) // Every 300 points, get faster
{
    // Show all panels by default
    showScorePoints = true;
    showHiScorePoints = true;
    showSpeedValue = true;

    // Set initial locations for text on screen
    scorePanelPosition = { 9, 3 };
    hiScorePanelPosition = { 32, 3 };
    snakeSpeedPanelPosition = { 56, 3 };

    // Initialize values
    scorePanelPoints = 0;
    hiScorePanelPoints = Game::lastHiScorePoints; // Load high score
    
    // Set speed to the slowest setting (last item in vector)
    speedTimeIndex = static_cast<ushort>(SnakeSpeedTimes.size()) - 1;
    nextSnakeSpeedTime = SnakeSpeedTimes[speedTimeIndex];
    snakeSpeedPanelValue = nextSnakeSpeedTime;
}

// Called when the stage starts to draw the UI layout
void UI::setupUI()
{
    drawUI();
}

// Called when Snake eats food
void UI::addScorePoints(ushort pointsToAdd)
{
    deleteUItimers(); // Stop any blinking effects currently running

    nextScoreValue += pointsToAdd;
    // Make score blink to show feedback
    blinkMethods.push_back(&UI::blinkScore_callBack);

    // If we beat the high score, blink that too!
    if (hiScorePanelPoints < nextScoreValue)
    {
        blinkMethods.push_back(&UI::blinkHiScore_callBack);
        Game::lastHiScorePoints = nextScoreValue;
    }

    // Logic to increase difficulty (Speed)
    // If we haven't reached max speed AND passed the point threshold...
    if (nextSnakeSpeedTime > SnakeSpeedTimes[0] &&
        (nextScoreValue % PointsToIncreaseSpeed) == 0)
    {
        // Increase speed index (move to faster time)
        nextSnakeSpeedTime = SnakeSpeedTimes[--speedTimeIndex];
        
        // Blink the speed panel so player notices
        blinkMethods.push_back(&UI::blinkSpeedValue_callBack);
    }

    // Set a timer to stop the blinking effects after 1 second
    Timer::setTimerAndCallback(1000, this, &UI::prepareToStopBlinking_callBack);

    // Start the actual blinking (toggle visibility every 100ms)
    for (auto method : blinkMethods)
        Timer::setTimerAndCallback(100, this, method);
}

// Helper to clean up timers when game ends or updates
void UI::deleteUItimers()
{
    Timer::markTimerForDeletion(this, &UI::prepareToStopBlinking_callBack);
    Timer::markTimerForDeletion(this, &UI::stopBlinking_callBack);

    for (auto method : blinkMethods)
        Timer::markTimerForDeletion(this, method);

    blinkMethods.clear();
    writeAllPanelValues(); // Ensure numbers are visible (not hidden mid-blink)
}

// Draws the static borders and labels
void UI::drawUI()
{
    // ... (Code that prints borders using '+', '-', '|') ...
    // Note: This uses coordinate math to align boxes perfectly.
    
    // Draw the labels: SCORE, HI-SCORE, SPEED
    Game::setCursorPosition(scorePanelPosition);
    cout << Stage::BorderCharacter << " SCORE: ";
    // ...
    
    writeAllPanelValues(); // Fill in the '0's
}

// The function that actually makes text blink (called by Timer)
void UI::blinkPanelValue(bool& showPanelValue, const ConsoleColor& valueForegroundColor)
{
    showPanelValue = !showPanelValue; // Toggle true/false
    
    if (showPanelValue)
        Game::setTextColors(BackgroundColor, valueForegroundColor); // Show Color
    else
        Game::setTextColors(BackgroundColor, BackgroundColor);      // Hide (make text same color as background)
}

// Utility to print numbers with leading zeros (e.g., "0050")
void UI::writePanelValue(const Point& panelPosition, const uchar setW, const ushort& panelValue)
{
    Game::setCursorPosition(panelPosition);
    cout << setfill('0') << setw(setW) << panelValue;
}