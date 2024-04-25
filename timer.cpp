#include "timer.h"
#include "qtimer.h"

Timer::Timer()
{
    QTimer *gametimer=new QTimer();
    gametimer->setInterval(1000);
    QObject::connect(gametimer, &QTimer::timeout, [&]() {
        counter++;
    });

}
