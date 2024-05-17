#include "player.h"
#include <QDebug>
#include <QFile>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QPropertyAnimation>
#include "coin.h"
#include "enemy.h"
#include "platform.h"
#include <QStandardPaths>

const qreal gravity = 15;
const qreal jumpVelocity = -85;

// constructor
Player::Player(QGraphicsScene *scene)
    : QGraphicsPixmapItem()
    , health(1)
    , coins(0)
    , yVelocity(0)
    , in_air(false)
{


    QString desktopDir = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString filePath = desktopDir + "/skinData.txt";
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << file.errorString();
        return;
    }

    QTextStream in(&file);
    QString line;
    QString equippedSkinImagePath;

    while (!(line = in.readLine()).isNull()) {
        QStringList fields = line.split(',');
        if (fields[3] == "TRUE") { // Check if the skin is equipped
            equippedSkinImagePath = fields[4].trimmed();
            equippedSkinImagePath = equippedSkinImagePath.replace("\"", "");
            break;
        }
    }

    file.close();

    // set the pixmap using the image path of the equipped skin
    qDebug() << equippedSkinImagePath;
    if (!equippedSkinImagePath.isEmpty()) {
        QPixmap pixmap(equippedSkinImagePath);
        if (!pixmap.isNull()) {
            setPixmap(pixmap.scaled(75, 75));
        } else {
            setPixmap(QPixmap(":/images/square.png").scaled(75, 75)); // fallback image if the pixmap is null
        }
    } else {
        setPixmap(QPixmap(":/images/square.png").scaled(75, 75)); // fallback image if no skin is equipped
    }
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();


    coinDisplay = new QGraphicsTextItem();
    coinDisplay->setPlainText("Coins: " + QString::number(coins));
    coinDisplay->setDefaultTextColor(QColor(255, 255, 255));
    coinDisplay->setPos(30, 10);
    scene->addItem(coinDisplay);

    msgBox = new QMessageBox();

    in_air = false;

    rotationTimer = new QTimer(this);
    connect(rotationTimer, &QTimer::timeout, this, &::Player::rotation);

    particleTimer = new QTimer(this);
    connect(particleTimer, &QTimer::timeout, this, &Player::emitParticles);
    particleTimer->start(500);
}

// this function increases coins and displays the number of coins
void Player::increase()
{
    coins++;
    coinDisplay->setPlainText("Coins: " + QString::number(coins));
}

// this function allows the player to jump using the space bar key
void Player::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Space && !in_air) {
        in_air = true;
        yVelocity += jumpVelocity;
        advance();
        rotation();
    }
}

// this function sets the variable in_air
void Player::setInAir(bool inAir)
{
    in_air = inAir;
}

// this function advances the player
void Player::advance()
{
    yVelocity += gravity;

    qreal newY = y() + yVelocity;
    qreal sceneHeight = scene()->sceneRect().height();

    if (newY < 0) {
        newY = 0;
        yVelocity = 0;
    } else if (newY > sceneHeight - pixmap().height() - 200) {
        newY = sceneHeight - pixmap().height() - 200;
        yVelocity = 0;
        in_air = false;
        emitParticles();
        particleTimer->start(500);
    } else
        particleTimer->stop();

    setPos(x(), newY);
    emit inAirChanged(in_air);
}

// this function creates an enemy
void Player::createEnemy()
{
    Enemy *enemy = new Enemy(*this);
    scene()->addItem(enemy);
}


// this function creates a coin
void Player::createCoin()
{
    coin *Coin = new coin();
    scene()->addItem(Coin);
}

// this function creates a platform
void Player::createPlatform()
{
    int randomX = QRandomGenerator::global()->bounded(600, 800 - pixmap().width());

    // make sure that the platform isn't colliding with any enemies on spawn
    QList<QGraphicsItem*> collidingItems;
    do {
        Platform *block = new Platform(); // create a new platform instance
        block->setPos(randomX + 1300, 540); // set the position of the platform
        collidingItems = block->collidingItems(); // check for collisions with enemies
        if (!collidingItems.isEmpty()) {
            randomX = QRandomGenerator::global()->bounded(400, 600 - pixmap().width());
        }
        else {
            scene()->addItem(block); // add the platform to the scene if there are no collisions
        }
    } while (!collidingItems.isEmpty()); // repeat until no collisions are detected
}

// this function checks whether the player is in the air
bool Player::isInAir() const
{
    return in_air;
}

// this function gets the y velocity
qreal Player::getYVelocity() const
{
    return yVelocity;
}

// this function set the yvelocity
void Player::setYVelocity(qreal velocity)
{
    yVelocity = velocity;
}

// this function resets the coins
void Player::resetCoins()
{
    coins = 0;
    coinDisplay->setPlainText("Coins: " + QString::number(coins));
}

// this function allows the player to rotate
void Player::rotation()
{
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

// this function sets the position
void Player::setPosition(qreal x, qreal y)
{
    setPos(x, y);
}

// this function shows the attempts
void Player::showAttempts()
{
    QGraphicsTextItem *attemptsText = new QGraphicsTextItem(
        QString("Attempt %1").arg(this->numOfAttempts));

    QFont font;
    font.setPointSize(40);
    font.setBold(true);
    attemptsText->setFont(font);

    attemptsText->setDefaultTextColor(Qt::white);

    int textWidth = attemptsText->boundingRect().width();
    int playerX = this->x();                                        // get player's x position
    attemptsText->setPos(playerX - textWidth / 2, this->y() - 200); // position above the player
    scene()->addItem(attemptsText);

    // create a QTimer to start moving the text item after a delay
    QTimer::singleShot(1000, [=]() {
        // create a QTimer to update the position every 30 milliseconds
        QTimer *timer = new QTimer(this);
        connect(timer, &QTimer::timeout, [=]() {
            attemptsText->moveBy(-15, 0); // move the text item 15 pixels to the left
            if (attemptsText->pos().x() + textWidth < 0) { // if the text item is out of scene
                scene()->removeItem(attemptsText);
                delete attemptsText;
                timer->stop();        // stop the timer
                timer->deleteLater(); // delete the timer
            }
        });
        timer->start(30); // start the timer with a 30-millisecond interval
    });

    resetCoins();
}

// this function creates a trail of particles
void Player::emitParticles()
{
    qreal particleY = y()
                      + pixmap().height(); // y coordinate for all particles (bottom of the player)
    qreal particleSize = QRandomGenerator::global()
                             ->bounded(5, 15); // randomize the size of the particles

    QGraphicsRectItem *particle = new QGraphicsRectItem(0,
                                                        0,
                                                        particleSize,
                                                        particleSize); // create a particle

    qreal particleX
        = x()
          - 10; // set position to be slightly to the left of the bottom-left corner of the player
    particle->setPos(particleX, particleY);

    particle->setBrush(Qt::blue); // set brush color

    scene()->addItem(particle); // add the particle to the scene

    qreal particleYVelocity = QRandomGenerator::global()
                                  ->bounded(-20, -5); // randomize the vertical speed of the particle
    particle->setY(particle->y() + particleYVelocity); // apply initial velocity to particles

    QTimer *moveTimer = new QTimer(this); // start a timer to update the particle's position
    connect(moveTimer, &QTimer::timeout, [=]() {
        particle->setX(particle->x() - 1); // move the particle to the left

        try {
            qreal distance = qAbs(
                particle->x()
                - x()); // calculate the absolute distance between the particle and the player

            if (distance > 50) { // check if the distance is greater than 100 pixels
                delete particle;
                moveTimer->stop(); // stop the timer
                delete moveTimer;  // delete the timer
            }
        } catch (std::exception const &e) {
            qDebug() << "Exception occurred: " << e.what();
            moveTimer->stop(); // stop the timer
            delete moveTimer;  // delete the timer
        }
    });
    moveTimer->start(10); // update the particle's position every 10 milliseconds
}
