#ifndef TIMER_H
#define TIMER_H

#include <QObject>

class Timer
{
public:
    Timer();
    void startTimer();
    void stopTimer();
    int counter=0;
};

#endif // TIMER_H
