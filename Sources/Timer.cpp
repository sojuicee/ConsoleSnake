#include "../Headers/Timer.h"
#include <algorithm> // For finding/removing items in vectors
#include <iostream>

using namespace std;

// Initialize the static variables defined in the Header
bool Timer::isRunning = false;
// Stores the exact time of the last "tick"
chrono::high_resolution_clock::time_point Timer::previousTimePoint;

// A waiting list for new timers that need to be added
vector<callbackAndTimersTuple> Timer::methodsAndTimersToAdd;
// The active list of timers currently running
vector<callbackAndTimersTuple> Timer::callbackMethodsAndTimers;

// Private function to kickstart the clock
void Timer::startTimer()
{
    // Get the current time right now
    previousTimePoint = chrono::high_resolution_clock::now();
    isRunning = true;
}

// THE MAIN LOOP: This is called constantly by the Game loop
bool Timer::run()
{
    using namespace chrono;

    // If there are no timers active and no new ones waiting, do nothing
    if (methodsAndTimersToAdd.empty() && callbackMethodsAndTimers.empty())
    {
        isRunning = false;
        return isRunning;
    }

    // If we have work to do but the clock is off, start it
    if (!isRunning)
        startTimer();

    if (isRunning)
    {
        // Calculate how many milliseconds passed since the last frame
        auto msSpan = duration_cast<milliseconds>(high_resolution_clock::now() - previousTimePoint);

        // If at least 1 millisecond has passed...
        if (msSpan.count() >= 1)
        {
            // Reset the clock for the next tick
            previousTimePoint = high_resolution_clock::now();

            // Check every active timer
            for (auto& tuple : callbackMethodsAndTimers)
            {
                // Get the current countdown value (by reference)
                long& currentCounter = get(tuple);
                bool& markedForDeletion = get(tuple);

                // If this timer is dead or hasn't reached 0 yet, skip it
                if (markedForDeletion || --currentCounter > 0)
                    continue;

                // TIME'S UP! Get the function pointer and object
                void* ownerObject = get(tuple);
                void (*methodPtr)(void*) = get(tuple);
                long& originalCounter = get(tuple);

                // Call the function (e.g., move the snake!)
                methodPtr(ownerObject);

                // Reset the countdown so it happens again (Looping timer)
                currentCounter = originalCounter;
            }
        }

        // CLEANUP: Remove timers that were marked for deletion
        for (size_t idx = callbackMethodsAndTimers.size(); idx > 0; idx--)
        {
            // If marked as true (delete me)...
            if (get(callbackMethodsAndTimers[idx - 1]))
                // Remove it from the vector
                callbackMethodsAndTimers.erase(callbackMethodsAndTimers.begin() + (idx - 1));
        }

        // ADD NEW: Move waiting timers into the active list
        for (auto& tuple : methodsAndTimersToAdd)
            callbackMethodsAndTimers.push_back(tuple);
            
        methodsAndTimersToAdd.clear(); // Clear the waiting list
    }

    return isRunning;
}

// Function to register a new task (e.g., "Snake, Move, every 200ms")
void Timer::setTimerAndCallback(long timerInMilliSeconds, void* ownerObject, void(*methodPtr)(void* ownerObject))
{
    // First, check if this timer already exists. If so, just update its time.
    for (auto& tuple : callbackMethodsAndTimers)
    {
        if (get(tuple) == ownerObject && get(tuple) == methodPtr)
        {
            get(tuple) = timerInMilliSeconds; // Update duration
            get(tuple) = false;               // Un-delete it if it was marked
            return;
        }
    }

    // If it's new, add it to the waiting list
    if (methodPtr != nullptr)
        methodsAndTimersToAdd.push_back(make_tuple(ownerObject, methodPtr, timerInMilliSeconds, timerInMilliSeconds, false));
}

// Function to stop a task (e.g., "Stop blinking the text")
void Timer::markTimerForDeletion(void* ownerObject, void(*methodPtr)(void* ownerObject))
{
    // Find the timer and mark the boolean flag as 'true'
    for (auto& tuple : callbackMethodsAndTimers)
    {
        if (get(tuple) == ownerObject && get(tuple) == methodPtr)
        {
            get(tuple) = true; // Mark for deletion
            return;
        }
    }
}

// Stop everything (used when Game Over or resetting)
void Timer::clearAll()
{
    methodsAndTimersToAdd.clear();
    callbackMethodsAndTimers.clear();
}