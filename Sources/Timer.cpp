#include "../Headers/Timer.h"
#include <algorithm>
#include <iostream>

using namespace std;

bool Timer::isRunning = false;
chrono::high_resolution_clock::time_point Timer::previousTimePoint;
vector<callbackAndTimersTuple> Timer::methodsAndTimersToAdd;
vector<callbackAndTimersTuple> Timer::callbackMethodsAndTimers;

void Timer::startTimer()
{
    previousTimePoint = chrono::high_resolution_clock::now();
    isRunning = true;
}

bool Timer::run()
{
    using namespace chrono;

    if (methodsAndTimersToAdd.empty() && callbackMethodsAndTimers.empty()) {
        isRunning = false;
        return isRunning;
    }

    if (!isRunning) startTimer();

    if (isRunning) {
        auto msSpan = duration_cast<milliseconds>(high_resolution_clock::now() - previousTimePoint);

        if (msSpan.count() >= 1) {
            previousTimePoint = high_resolution_clock::now();

            for (auto& tuple : callbackMethodsAndTimers) {
                // IMPORTANT: Using std::get with Index
                long& currentCounter = get(tuple);
                bool& markedForDeletion = get(tuple);

                if (markedForDeletion || --currentCounter > 0)
                    continue;

                void* ownerObject = get(tuple);
                void (*methodPtr)(void*) = get(tuple);
                long& originalCounter = get(tuple);

                methodPtr(ownerObject);
                currentCounter = originalCounter;
            }
        }

        for (size_t idx = callbackMethodsAndTimers.size(); idx > 0; idx--) {
            if (get(callbackMethodsAndTimers[idx - 1]))
                callbackMethodsAndTimers.erase(callbackMethodsAndTimers.begin() + (idx - 1));
        }

        for (auto& tuple : methodsAndTimersToAdd)
            callbackMethodsAndTimers.push_back(tuple);
        methodsAndTimersToAdd.clear();
    }
    return isRunning;
}

void Timer::setTimerAndCallback(long timerInMilliSeconds, void* ownerObject, void(*methodPtr)(void* ownerObject))
{
    for (auto& tuple : callbackMethodsAndTimers) {
        if (get(tuple) == ownerObject && get(tuple) == methodPtr) {
            get(tuple) = timerInMilliSeconds;
            get(tuple) = false;
            return;
        }
    }
    if (methodPtr != nullptr)
        methodsAndTimersToAdd.push_back(make_tuple(ownerObject, methodPtr, timerInMilliSeconds, timerInMilliSeconds, false));
}

void Timer::markTimerForDeletion(void* ownerObject, void(*methodPtr)(void* ownerObject))
{
    for (auto& tuple : callbackMethodsAndTimers) {
        if (get(tuple) == ownerObject && get(tuple) == methodPtr) {
            get(tuple) = true;
            return;
        }
    }
}

void Timer::clearAll()
{
    methodsAndTimersToAdd.clear();
    callbackMethodsAndTimers.clear();
}