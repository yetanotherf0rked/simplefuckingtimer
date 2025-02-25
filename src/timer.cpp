#include "timer.h"

Timer::Timer() : state(STOPPED), elapsed(0) {}

void Timer::start() {
    std::lock_guard<std::mutex> lock(mtx);
    if (state == STOPPED) {
        startTime = std::chrono::steady_clock::now();
        state = RUNNING;
    }
}

void Timer::split() {
    std::lock_guard<std::mutex> lock(mtx);
    if (state == RUNNING) {
        elapsed = std::chrono::steady_clock::now() - startTime;
        state = FINISHED;
    }
}

void Timer::reset() {
    std::lock_guard<std::mutex> lock(mtx);
    state = STOPPED;
    elapsed = std::chrono::duration<double>(0);
}

void Timer::pause() {
    std::lock_guard<std::mutex> lock(mtx);
    if (state == RUNNING) {
        pausedTime = std::chrono::steady_clock::now();
        // Capture the elapsed time at pause
        elapsed = pausedTime - startTime;
        state = PAUSED;
    }
}


void Timer::resume() {
    std::lock_guard<std::mutex> lock(mtx);
    if (state == PAUSED) {
        startTime += (std::chrono::steady_clock::now() - pausedTime);
        state = RUNNING;
    }
}

double Timer::getTime() {
    std::lock_guard<std::mutex> lock(mtx);
    if (state == RUNNING)
        return std::chrono::duration<double>(std::chrono::steady_clock::now() - startTime).count();
    else
        return elapsed.count();
}

TimerState Timer::getState() {
    std::lock_guard<std::mutex> lock(mtx);
    return state;
}

