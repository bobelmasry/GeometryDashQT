#ifndef COIN_H
#define COIN_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QTimer>
#include <QRandomGenerator>

class coin : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    coin();
public slots:
    void move();
};

#endif // COIN_H
