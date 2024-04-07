#include "player.h"
#include <QKeyEvent>
#include <QGraphicsScene>
#include <QDebug>
#include "enemy.h"
#include "coin.h"
#include "level1.h"
#include <QGraphicsRectItem>


const qreal gravity = 15;
const qreal jumpVelocity = -80;

Player::Player(QGraphicsScene *scene) : QGraphicsRectItem(), health(1), coins(0), yVelocity(0) {
    setRect(0, 0, 100, 100);

    setBrush(Qt::blue);

    setFlag(QGraphicsItem::ItemIsFocusable);

    setFocus();

    coinDisplay = new QGraphicsTextItem();
    coinDisplay->setPlainText("Coins: " + QString::number(coins));
    coinDisplay->setDefaultTextColor(QColor(255,255,255));
    coinDisplay->setPos(30, 10);
    scene->addItem(coinDisplay);

    msgBox = new QMessageBox();

    in_air=false;

    rotationTimer = new QTimer(this);
    connect(rotationTimer, &QTimer::timeout, this, &::Player::rotation);

    particleTimer = new QTimer(this);
    connect(particleTimer, &QTimer::timeout, this, &Player::emitParticles);
    particleTimer->start(500);
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
    if (event->key() == Qt::Key_Space && !in_air) {
        in_air = true;
        yVelocity += jumpVelocity;
        rotation();
    }
}


void Player::advance()
{
        yVelocity += gravity;

        qreal newY = y() + yVelocity;
        qreal sceneHeight = scene()->sceneRect().height();

        if (newY < 0) {
            newY = 0;
            yVelocity = 0;
        } else if (newY > sceneHeight - rect().height() - 200) {
            newY = sceneHeight - rect().height() - 200;
            yVelocity = 0;
            in_air=false;
            emitParticles();
            particleTimer->start(500);
        }
        else
            particleTimer->stop();

        setPos(x(), newY);


        QList<QGraphicsItem*> colliding_items = collidingItems();
        for (int i = 0; i < colliding_items.size(); ++i)
        {
            if (typeid(*(colliding_items[i])) == typeid(level1))
            {

            }
        }
}

void Player::createEnemy()
{
  Enemy* enemy = new Enemy();
  scene()->addItem(enemy);
}

/*
void Player::createCoin()
{
    coin* Coin = new coin();
    scene()->addItem(Coin);
}*/


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

void Player::emitParticles()
{
    qreal particleY = y() + rect().height(); // y coordinate for all particles (bottom of the player)
    qreal particleSize = QRandomGenerator::global()->bounded(5, 15); // randomize the size of the particles

    QGraphicsRectItem *particle = new QGraphicsRectItem(0, 0, particleSize, particleSize); // create a particle

    qreal particleX = x() - 10; // set position to be slightly to the left of the bottom-left corner of the player
    particle->setPos(particleX, particleY);

    particle->setBrush(Qt::blue); // set brush color

    scene()->addItem(particle); // add the particle to the scene

    qreal particleYVelocity = QRandomGenerator::global()->bounded(-20, -5); // randomize the vertical speed of the particle
    particle->setY(particle->y() + particleYVelocity); // apply initial velocity to particles

    QTimer *moveTimer = new QTimer(this); // start a timer to update the particle's position
    connect(moveTimer, &QTimer::timeout, [=]() {
        particle->setX(particle->x() - 1); // move the particle to the left

        qreal distance = qAbs(particle->x() - x()); // calculate the absolute distance between the particle and the player

        if (distance > 50) { // check if the distance is greater than 100 pixels
            scene()->removeItem(particle); // remove the particle from the scene and delete it
            delete particle;
            moveTimer->stop(); // stop the timer
            delete moveTimer; // delete the timer
        }
    });
    moveTimer->start(10); // update the particle's position every 10 milliseconds
}




