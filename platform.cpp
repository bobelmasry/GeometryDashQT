#include "platform.h"
#include "player.h"
#include <QGraphicsScene>
#include <QTimer>
#include <QList>
#include <QDebug>
#include "coin.h"
#include "enemy.h"
#include "level2.h"

QList<Platform *> Platform::platforms;

int Platform::continious_plats=0;

Platform::Platform()
{
    platforms.append(this);

    if(++continious_plats<100)
    {
        continious_plats++;
        qDebug()<<continious_plats;
    }

    else if(continious_plats>100)
    {
        level2::level_complete();
        continious_plats=0;
    }

    QGraphicsItemGroup *platformGroup = new QGraphicsItemGroup();

    int rectHeight = 70; // Fixed height for the rectangle
    int squareHeight = 10; // Fixed height for the square on top of the rectangle
    int gap = 20; // Small gap between the rectangle and the square to ensure separate collisions

    QGraphicsRectItem *rectangle = new QGraphicsRectItem(0, 60, 100, rectHeight);

    QGraphicsRectItem *square = new QGraphicsRectItem(0, 60 - squareHeight - gap, 100, squareHeight);

    rectangle->setBrush(QColor(255, 20, 147));
    square->setBrush(QColor(255, 20, 147));
    rectangle->setPen(QPen(Qt::blue));
    square->setPen(QPen(Qt::blue));

    death_sound = new QMediaPlayer();
    death_audio = new QAudioOutput();
    death_sound->setSource(QUrl("qrc:/Sound/death_sound.mp3"));
    death_sound->setAudioOutput(death_audio);
    death_audio->setVolume(50);

    platformGroup->addToGroup(rectangle);
    platformGroup->addToGroup(square);
    addToGroup(platformGroup);

    setPos(1600, 610 - rectHeight);


    QTimer *movement_timer = new QTimer(this);
    connect(movement_timer, SIGNAL(timeout()), this, SLOT(move()));
    movement_timer->start(30);
}

void Platform::move()
{
    setPos(x() - 15, y());

    if (x() + boundingRect().width() < boundingRect().width()) {
        scene()->removeItem(this);
        platforms.removeOne(this);
        delete this;
        return;
    }

    QGraphicsItemGroup *platformGroup = static_cast<QGraphicsItemGroup *>(childItems().at(0));
    QGraphicsRectItem *rectangle = static_cast<QGraphicsRectItem *>(platformGroup->childItems().at(0));
    QGraphicsRectItem *square = static_cast<QGraphicsRectItem *>(platformGroup->childItems().at(1));

    QList<QGraphicsItem *> colliding_items = collidingItems();

    bool playerLanded = false;

    for (int i = 0; i < colliding_items.size(); ++i) {
        if (typeid(*(colliding_items[i])) == typeid(Player)) {
            Player *player = dynamic_cast<Player *>(colliding_items[i]);

            if (square->collidesWithItem(player)) {
                //qDebug() << "Collision with square detected";
                if (player->getYVelocity() > 0) { // Only if the player is falling
                    player->setYVelocity(-5); // Reset vertical velocity
                    player->setInAir(false); // Player is no longer in the air
                    player->setPos(player->x(), square->scenePos().y() - player->pixmap().height());
                    //qDebug() << "Player landed on the square";
                }
                playerLanded = true;
            } else if (rectangle->collidesWithItem(player)) {
               // qDebug() << "Collision with rectangle detected";
                player->setPosition(300,500);
                player->numOfAttempts++;
                player->showAttempts();
                death_sound->play();
                player_hit();
                return;
            }
        }
    }

    if (!playerLanded) {
        // If the player is no longer colliding with the platform, make them fall
        for (Platform* platform : Platform::platforms) {
            QList<QGraphicsItem *> collidingWithPlatform = platform->collidingItems();
            for (QGraphicsItem *item : collidingWithPlatform) {
                if (typeid(*item) == typeid(Player)) {
                    Player *player = dynamic_cast<Player *>(item);
                    player->setInAir(true);
                }
            }
        }
    }
}

void Platform::player_hit()
{


    if (level2::level2_music) {
        level2::level2_music->stop(); // Stop the music
        QTimer::singleShot(1000, []() {
            level2::level2_music->play(); // Play the music after a delay of 1000 milliseconds (1 second)
        });

    }

    continious_plats=0;

    foreach(Platform*platform, platforms){
        if(platform->scene())
            platform->scene()->removeItem(platform);
        delete platform;
    }

    platforms.clear();

    Enemy::continous_enemies=0;

    foreach(Enemy*enemy, Enemy::enemies){
        if(enemy->scene())
            enemy->scene()->removeItem(enemy);
        delete enemy;
    }

    Enemy::enemies.clear();

    foreach(coin*Coin,coin::coins){
        if(Coin->scene())
            Coin->scene()->removeItem(Coin);
        delete Coin;
    }
    coin::coins.clear();

}

Platform::~Platform() {}
