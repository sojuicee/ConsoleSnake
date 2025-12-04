#include "Timer.h"
#include <algorithm> // For vector operations
#include <iostream>

using namespace std;

// Initialize static members
bool Timer::isRunning = false;
chrono::high_resolution_clock::time_point Timer::previousTimePoint;
vector<callbackAndTimersTuple> Timer::methodsAndTimersToAdd;
vector<callbackAndTimersTuple> Timer::callbackMethodsAndTimers;

// Private method to start the timer
void Timer::startTimer()
{
    previousTimePoint = chrono::high_resolution_clock::now();
    isRunning = true;
}

// Run all timers
bool Timer::run()
{
    using namespace chrono;

    // If no timers, stop running
    if (methodsAndTimersToAdd.empty() && callbackMethodsAndTimers.empty())
    {
        isRunning = false;
        return isRunning;
    }

    if (!isRunning)
        startTimer();

    if (isRunning)
    {
        // Time elapsed since last tick in milliseconds
        auto msSpan = duration_cast<milliseconds>(high_resolution_clock::now() - previousTimePoint);

        if (msSpan.count() >= 1)
        {
            previousTimePoint = high_resolution_clock::now();

            // Update all active timers
            for (auto& tuple : callbackMethodsAndTimers)
            {
                long& currentCounter = get<2>(tuple);
                bool& markedForDeletion = get<4>(tuple);

                if (markedForDeletion || --currentCounter > 0)
                    continue;

                void* ownerObject = get<0>(tuple);
                void (*methodPtr)(void*) = get<1>(tuple);
                long& originalCounter = get<3>(tuple);

                // Call the method
                methodPtr(ownerObject);

                // Reset counter
                currentCounter = originalCounter;
            }
        }

        // Remove timers marked for deletion
        for (size_t idx = callbackMethodsAndTimers.size(); idx > 0; idx--)
        {
            if (get<4>(callbackMethodsAndTimers[idx - 1]))
                callbackMethodsAndTimers.erase(callbackMethodsAndTimers.begin() + (idx - 1));
        }

        // Add new timers
        for (auto& tuple : methodsAndTimersToAdd)
            callbackMethodsAndTimers.push_back(tuple);
        methodsAndTimersToAdd.clear();
    }

    return isRunning;
}

// Add or update a timer
void Timer::setTimerAndCallback(long timerInMilliSeconds, void* ownerObject, void(*methodPtr)(void* ownerObject))
{
    // Update existing timer if present
    for (auto& tuple : callbackMethodsAndTimers)
    {
        if (get<0>(tuple) == ownerObject && get<1>(tuple) == methodPtr)
        {
            get<3>(tuple) = timerInMilliSeconds;
            get<4>(tuple) = false;
            return;
        }
    }

    // Add new timer
    if (methodPtr != nullptr)
        methodsAndTimersToAdd.push_back(make_tuple(ownerObject, methodPtr, timerInMilliSeconds, timerInMilliSeconds, false));
}

// Mark a timer for deletion
void Timer::markTimerForDeletion(void* ownerObject, void(*methodPtr)(void* ownerObject))
{
    for (auto& tuple : callbackMethodsAndTimers)
    {
        if (get<0>(tuple) == ownerObject && get<1>(tuple) == methodPtr)
        {
            get<4>(tuple) = true;
            return;
        }
    }
}

// Clear all timers
void Timer::clearAll()
{
    methodsAndTimersToAdd.clear();
    callbackMethodsAndTimers.clear();
}
