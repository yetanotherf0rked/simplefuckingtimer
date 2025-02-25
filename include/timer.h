#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include <mutex>

enum TimerState { STOPPED, RUNNING, PAUSED, FINISHED };

class Timer {
public:
    Timer();
    void start();
    void split();
    void reset();
    void pause();
    void resume();
    void revertFinish(); // New: Allows undoing a finished run
    double getTime();
    TimerState getState();
    
private:
    TimerState state;
    std::chrono::steady_clock::time_point startTime;
    std::chrono::steady_clock::time_point pausedTime;
    std::chrono::duration<double> elapsed;
    std::mutex mtx;
};

#endif // TIMER_H

