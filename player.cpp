#include "player.h"
#include <QKeyEvent>
#include <QGraphicsScene>
#include <QDebug>
#include "enemy.h"
#include "coin.h"
#include <QGraphicsRectItem>
#include <QPropertyAnimation>
#include <QFile>


const qreal gravity = 15;
const qreal jumpVelocity = -85;

Player::Player(QGraphicsScene *scene) : QGraphicsPixmapItem(), health(1), coins(0), yVelocity(0) {
    QFile file(":/images/skinData.csv");
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

    // Set the pixmap using the image path of the equipped skin
    qDebug() << equippedSkinImagePath;
    if (!equippedSkinImagePath.isEmpty()) {
        setPixmap(QPixmap(equippedSkinImagePath).scaled(100, 100));
    } else {
        qDebug() << "No skin is currently equipped.";
    }    setFlag(QGraphicsItem::ItemIsFocusable);

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
        } else if (newY > sceneHeight - pixmap().height() - 200) {
            newY = sceneHeight - pixmap().height() - 200;
            yVelocity = 0;
            in_air=false;
            emitParticles();
            particleTimer->start(500);
        }
        else
            particleTimer->stop();

        setPos(x(), newY);
}

void Player::createEnemy()
{
   Enemy* enemy = new Enemy(*this);
   scene()->addItem(enemy);
}


void Player::createCoin()
{
    coin* Coin = new coin();
    scene()->addItem(Coin);
}

void Player::resetCoins(){
    coins = 0;
    coinDisplay->setPlainText("Coins: " + QString::number(coins));
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

void Player::setPosition(qreal x, qreal y)
{
    setPos(x, y);
}

void Player::showAttempts() {
    QGraphicsTextItem *attemptsText = new QGraphicsTextItem(QString("Attempt %1").arg(this->numOfAttempts));

    QFont font;
    font.setPointSize(40);
    font.setBold(true);
    attemptsText->setFont(font);

    attemptsText->setDefaultTextColor(Qt::white);

    int textWidth = attemptsText->boundingRect().width();
    int playerX = this->x(); // Get player's x position
    attemptsText->setPos(playerX - textWidth / 2, this->y() - 200); // Position above the player
    scene()->addItem(attemptsText);

    // Create a QTimer to start moving the text item after a delay
    QTimer::singleShot(1000, [=]() {
        // Create a QTimer to update the position every 30 milliseconds
        QTimer *timer = new QTimer(this);
        connect(timer, &QTimer::timeout, [=]() {
            attemptsText->moveBy(-15, 0); // Move the text item 15 pixels to the left
            if (attemptsText->pos().x() + textWidth < 0) { // If the text item is out of scene
                scene()->removeItem(attemptsText);
                delete attemptsText;
                timer->stop(); // Stop the timer
                timer->deleteLater(); // Delete the timer
            }
        });
        timer->start(30); // Start the timer with a 30-millisecond interval
    });

    resetCoins();
}



void Player::emitParticles()
{
    qreal particleY = y() + pixmap().height(); // y coordinate for all particles (bottom of the player)
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

        try {
            qreal distance = qAbs(particle->x() - x()); // calculate the absolute distance between the particle and the player

            if (distance > 50) { // check if the distance is greater than 100 pixels
                delete particle;
                moveTimer->stop(); // stop the timer
                delete moveTimer; // delete the timer
            }
        } catch (std::exception const& e) {
            qDebug() << "Exception occurred: " << e.what();
            moveTimer->stop(); // stop the timer
            delete moveTimer; // delete the timer
        }
    });
    moveTimer->start(10); // update the particle's position every 10 milliseconds
}






