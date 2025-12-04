#pragma once

#include <chrono>
#include <vector>
#include <tuple>

// Tuple structure for callback timers:
// (owner object pointer, method pointer, current timer, original timer, marked for deletion)
using callbackAndTimersTuple = tuple<void*, void(*)(void*), long, long, bool>;

class Timer
{
private:
    Timer(); // Private constructor to prevent instantiation
    static bool isRunning; // Is the timer running?
    static chrono::high_resolution_clock::time_point previousTimePoint; // Last tick
    static vector<callbackAndTimersTuple> methodsAndTimersToAdd; // Timers waiting to be added
    static vector<callbackAndTimersTuple> callbackMethodsAndTimers; // Active timers

    static void startTimer(); // Start timer

public:
    static bool run(); // Call repeatedly to update timers
    static void setTimerAndCallback(long timerInMilliSeconds, void* ownerObject, void(*methodPtr)(void* ownerObject));
    static void markTimerForDeletion(void* ownerObject, void(*methodPtr)(void* ownerObject));
    static void clearAll(); // Remove all timers
};
