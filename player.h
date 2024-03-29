#ifndef PLAYER_H
#define PLAYER_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QKeyEvent>
#include <QGraphicsTextItem>
#include <QMessageBox>

class Player : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
    int health;
    int coins;
    QGraphicsTextItem* coinDisplay;
    QMessageBox* msgBox;
    qreal yVelocity;

public:
    Player(QGraphicsScene *scene);
    void keyPressEvent(QKeyEvent *event);
    int getHealth();
public slots:
    void createEnemy();
    void createCoin();
    void decrease();
    void increase();
    void advance();
};

#endif // PLAYER_H
