#ifndef PLAYER_H
#define PLAYER_H

#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QKeyEvent>
#include <QMessageBox>
#include <QObject>
#include <QPropertyAnimation>
#include <QTimer>


class Platform;
class Player : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
    int health;
    int coins;
    QGraphicsTextItem *coinDisplay;
    QMessageBox *msgBox;
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
    void resetCoins();
    void createPlatform();

private:
    qreal angle;
    QTimer *rotationTimer;
    QPropertyAnimation *rotationAnimation;
    QTimer *particleTimer;
    bool in_air;
    QGraphicsTextItem *attemptsText;

};

#endif // PLAYER_H
