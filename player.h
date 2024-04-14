#ifndef PLAYER_H
#define PLAYER_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QKeyEvent>
#include <QGraphicsTextItem>
#include <QMessageBox>
#include <QTimer>
#include <QPropertyAnimation>

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
    int numOfAttempts = 1;
public slots:
    void createEnemy();
    void createCoin();
    void increase();
    void advance();
    void rotation();
    void emitParticles();
    void setPosition(qreal x, qreal y);
    void showAttempts();
private:
    qreal angle;
    QTimer* rotationTimer;
    QPropertyAnimation *rotationAnimation;
    QTimer* particleTimer;
    bool in_air;

};

#endif // PLAYER_H
