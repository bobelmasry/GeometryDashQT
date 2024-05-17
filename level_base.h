#ifndef LEVEL_BASE_H
#define LEVEL_BASE_H
#include "QGraphicsScene"
class level_base
{
public:
    level_base();
    static int level;
    static int elapsedSec;
    static void elapsed_timer_creator(QGraphicsScene *scene);
};

#endif // LEVEL_BASE_H
