#include "player.h"
#include <QKeyEvent>
#include <QGraphicsScene>
#include <QDebug>
#include "enemy.h"
#include <QGraphicsRectItem>

Player::Player(QGraphicsScene *scene) : QGraphicsRectItem(), health(1), coins(0) {
    setRect(0, 0, 50, 50);
    setPos(30, 720);

    setBrush(Qt::blue);

    setFlag(QGraphicsItem::ItemIsFocusable);

    setFocus();

    coinDisplay = new QGraphicsTextItem();
    coinDisplay->setPlainText("Coins: " + QString::number(coins));
    coinDisplay->setDefaultTextColor(Qt::blue);
    coinDisplay->setPos(30, 10);
    scene->addItem(coinDisplay);

    msgBox = new QMessageBox();
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
    coinDisplay->setDefaultTextColor(Qt::blue);
}


void Player::keyPressEvent(QKeyEvent *event)
{

    if(event->key()== Qt::Key_Space)
    {
        setPos(x(),y()-20);    }
    }

void Player::createEnemy()
{
    Enemy* enemy = new Enemy();
    scene()->addItem(enemy);
}
