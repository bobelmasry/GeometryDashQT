#include "platform.h"
#include "player.h"
#include <QGraphicsScene>
#include <QTimer>
#include <QList>
#include <QDebug>
#include "coin.h"
#include "enemy.h"
#include "level_base.h"
#include "level1.h"
#include "level2.h"
#include "level3.h"
#include "level4.h"
#include "level5.h"

// list to store all platform objects created
QList<Platform *> Platform::platforms;

// variable to track continuous platforms
int Platform::continious_plats=0;

// constructor
Platform::Platform()
{
    platforms.append(this); // add this platform to the list of platforms

    // create a group for platform elements
    QGraphicsItemGroup *platformGroup = new QGraphicsItemGroup();


    int rectHeight = 70; // Fixed height for the rectangle
    int squareHeight = 10; // Fixed height for the square on top of the rectangle
    int gap = 30; // Small gap between the rectangle and the square to ensure separate collisions

    // create rectangle and square shapes
    QGraphicsRectItem *rectangle = new QGraphicsRectItem(0, 60, 70, rectHeight);
    QGraphicsRectItem *square = new QGraphicsRectItem(0, 60 - squareHeight - gap, 70, squareHeight);

    // set brush and pen colors for rectangle and square
    rectangle->setBrush(QColor(255, 20, 147));
    square->setBrush(QColor(255, 20, 147));
    rectangle->setPen(QPen(Qt::blue));
    square->setPen(QPen(Qt::blue));

    // setup death sound and audio output
    death_sound = new QMediaPlayer();
    death_audio = new QAudioOutput();
    death_sound->setSource(QUrl("qrc:/Sound/death_sound.mp3"));
    death_sound->setAudioOutput(death_audio);
    death_audio->setVolume(50);

    // add rectangle and square to the platform group
    platformGroup->addToGroup(rectangle);
    platformGroup->addToGroup(square);
    addToGroup(platformGroup);

    // set initial position of the platform
    setPos(1600, 610 - rectHeight);


    // setup a timer to move the platform
    QTimer *movement_timer = new QTimer(this);
    connect(movement_timer, SIGNAL(timeout()), this, SLOT(move()));
    movement_timer->start(30);



    //level 1 completetino condition

    if(++continious_plats<60&&level_base::level==1)
    {
        continious_plats++;
        qDebug()<<continious_plats;
    }

    else if(continious_plats>60&&level_base::level==1)
    {
         qDebug()<<"called level_complete";
        level1::level_complete();
        continious_plats=0;
    }


    //level 4 completion conditoins

    if(++continious_plats<200&&level_base::level==4)
    {
        continious_plats++;
        qDebug()<<continious_plats;
    }
    else if(++continious_plats>200&&level_base::level==4)
    {
        level4::level_complete();
        continious_plats=0;
    }

    // level 5 completion conditions

    if(++continious_plats<150&&level_base::level==5)
    {
        continious_plats++;
        qDebug()<<continious_plats;
    }
    else if(++continious_plats>150&&level_base::level==5)
    {
        level4::level_complete();
        continious_plats=0;
    }


}

// function to move the platform
void Platform::move()
{
    setPos(x() - 15, y()); // move the platform leftwards

    // check if the platform is out of the scene bounds
    if (x() + boundingRect().width() < boundingRect().width()) {
        scene()->removeItem(this); // remove the platform from the scene
        platforms.removeOne(this); // remove the platform from the list of platforms
        delete this; // delete the platform object
        return;
    }

    // get the rectangle and square from the platform group
    QGraphicsItemGroup *platformGroup = static_cast<QGraphicsItemGroup *>(childItems().at(0));
    QGraphicsRectItem *rectangle = static_cast<QGraphicsRectItem *>(platformGroup->childItems().at(0));
    QGraphicsRectItem *square = static_cast<QGraphicsRectItem *>(platformGroup->childItems().at(1));

    // check collisions with player
    QList<QGraphicsItem *> colliding_items = collidingItems();

    bool playerLanded = false;

    for (int i = 0; i < colliding_items.size(); ++i) {
        if (typeid(*(colliding_items[i])) == typeid(Player)) {
            Player *player = dynamic_cast<Player *>(colliding_items[i]);

            if (square->collidesWithItem(player)) {
                if (player->getYVelocity() > 0) { //only if the player is falling
                    player->setYVelocity(0); // reset vertical velocity
                    player->setInAir(false); // player is no longer in the air
                    player->setPos(player->x(), square->scenePos().y() - player->pixmap().height());
                }
                playerLanded = true;
            } else if (rectangle->collidesWithItem(player)) {
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
        // if the player is no longer colliding with the platform, make them fall
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

// function to handle player hit
void Platform::player_hit()
{

    // stop level music based on current level
    if (level1::level1_music&& level1::level1_music->isPlaying() == QMediaPlayer::PlayingState) {
        level1::level1_music->stop(); // Stop the music
        QTimer::singleShot(1000, []() {
            level1::level1_music->play(); // Play the music after a delay of 1000 milliseconds (1 second)
        });

    }

    if (level2::level2_music&& level2::level2_music->isPlaying() == QMediaPlayer::PlayingState) {
        level2::level2_music->stop(); // Stop the music
        QTimer::singleShot(1000, []() {
            level2::level2_music->play(); // Play the music after a delay of 1000 milliseconds (1 second)
        });

    }

    if (level3::level3_music&& level3::level3_music->isPlaying() == QMediaPlayer::PlayingState) {
        level3::level3_music->stop(); // Stop the music
        QTimer::singleShot(1000, []() {
            level3::level3_music->play(); // Play the music after a delay of 1000 milliseconds (1 second)
        });

    }

    if (level4::level4_music&& level4::level4_music->isPlaying() == QMediaPlayer::PlayingState) {
        level4::level4_music->stop(); // Stop the music
        QTimer::singleShot(1000, []() {
            level4::level4_music->play(); // Play the music after a delay of 1000 milliseconds (1 second)
        });

    }

    if (level5::level5_music && level5::level5_music->isPlaying() == QMediaPlayer::PlayingState) {
        level5::level5_music->stop(); // Stop the music
        QTimer::singleShot(1000, []() {
            level5::level5_music->play(); // Play the music after a delay of 1000 milliseconds (1 second)
        });
    }


    continious_plats=0;

    foreach(Platform*platform, platforms){
        if(platform->scene())
            platform->scene()->removeItem(platform);
        delete platform; // deletes platform
    }

    platforms.clear();

    Enemy::continous_enemies=0;

    foreach(Enemy*enemy, Enemy::enemies){
        if(enemy->scene())
            enemy->scene()->removeItem(enemy);
        delete enemy; // deletes enemy
    }

    Enemy::enemies.clear();

    foreach(coin*Coin,coin::coins){
        if(Coin->scene())
            Coin->scene()->removeItem(Coin);
        delete Coin; // deletes coin
    }
    coin::coins.clear();

}

Platform::~Platform() {}
