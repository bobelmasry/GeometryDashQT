#include "player.h"
#include <QKeyEvent>
#include <QGraphicsScene>
#include <QDebug>
#include "enemy.h"
#include "coin.h"
#include <QGraphicsRectItem>


const qreal gravity = 12;
const qreal jumpVelocity = -80;

Player::Player(QGraphicsScene *scene) : QGraphicsRectItem(), health(1), coins(0), yVelocity(0) {
    setRect(0, 0, 75, 75);

    setBrush(Qt::red);

    setFlag(QGraphicsItem::ItemIsFocusable);

    setFocus();

    coinDisplay = new QGraphicsTextItem();
    coinDisplay->setPlainText("Coins: " + QString::number(coins));
    coinDisplay->setDefaultTextColor(QColor(255,255,255));
    coinDisplay->setPos(30, 10);
    scene->addItem(coinDisplay);

    msgBox = new QMessageBox();

    rotationTimer = new QTimer(this);
    connect(rotationTimer, &QTimer::timeout, this, &::Player::rotation);
}

int Player::getHealth(){
    return health;
}

void Player::decrease() {
    health--;
    if (health < 1) {
        msgBox->setText("Game Over!\n Coins: " + QString::number(coins));
        msgBox->setWindowTitle("Game Over");
        msgBox->exec();
    }
}

void Player::increase() {
    coins++;
    coinDisplay->setPlainText("Coins: " + QString::number(coins));
}

void Player::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Space && !rotationTimer->isActive())
    {
        yVelocity += jumpVelocity;
        rotation();

    }
}

void Player::advance() {
        yVelocity += gravity;

        qreal newY = y() + yVelocity;
        qreal sceneHeight = scene()->sceneRect().height();

        if (newY < 0) {
            newY = 0;
            yVelocity = 0;
        } else if (newY > sceneHeight - rect().height() - 200) {
            newY = sceneHeight - rect().height() - 200;
            yVelocity = 0;
        }

        setPos(x(), newY);

}

void Player::createEnemy()
{
   Enemy* enemy = new Enemy();
   scene()->addItem(enemy);
}

void Player::createCoin()
{
    coin* Coin = new coin();
    scene()->addItem(Coin);
}

void Player::rotation() {
    static qreal currentRotation = 0;
    if (currentRotation == 0) {
        setTransformOriginPoint(boundingRect().center());

        QTimer *timer = new QTimer(this);
        connect(timer, &QTimer::timeout, [=]() {
            if (currentRotation < 90) {
                setRotation(currentRotation);
                currentRotation += 3;
            } else {
                setRotation(0);
                timer->stop();
                currentRotation = 0;
                delete timer;
            }
        });
        timer->start(15);
    }
}



