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
    void set_level();
    void update_timer();
};

#endif // LEVEL_BASE_H
