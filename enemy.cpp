#include "enemy.h"
#include "player.h"
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QPolygonF>
#include <coin.h>
#include <QApplication>
#include "platform.h"
#include "level_base.h"
#include "level2.h"
#include "level3.h"
#include "level4.h"
#include "level5.h"

QList<Enemy*>Enemy::enemies;

int Enemy::continous_enemies=0;

Enemy::Enemy(Player &player) : m_player(player) {

    enemies.append(this); // adds the current enemy object to the list of enemies


    // randomizes enemy shape and spikes
    int hasRect = QRandomGenerator::global()->bounded(0, 3);
    int randomRectHeight = QRandomGenerator::global()->bounded(30, 45);
    int numOfSpikes = QRandomGenerator::global()->bounded(0, 5);

    if (hasRect > 0) {
        randomRectHeight = 0;
    }

    // creates polygon for single spike shape
    QPolygonF polygon;
    polygon << QPointF(0, 60) << QPointF(30, 0) << QPointF(60, 60);
    QGraphicsPolygonItem *singlePolygon = new QGraphicsPolygonItem(polygon);
    singlePolygon->setBrush(QColor(255,20,147));
    singlePolygon->setPen(QPen(Qt::white));

    // create a group to hold the spikes
    QGraphicsItemGroup *spikesGroup = new QGraphicsItemGroup();

    if (hasRect > 0){
    for (int i = 0; i < numOfSpikes; ++i) {
        QPolygonF spikePolygon;
        spikePolygon << QPointF(0, 60) << QPointF(15, 0) << QPointF(30, 60);

        QGraphicsPolygonItem *spike = new QGraphicsPolygonItem(spikePolygon);
        spike->setBrush(QColor(255,20,147));
        spike->setPen(QPen(Qt::white));

        spike->setPos(i * 30, 0);

        spikesGroup->addToGroup(spike);
    }
    }

    if (hasRect == 0) {
        // creates a rectangle and adds it to the spikes group
        QGraphicsRectItem *rectangle = new QGraphicsRectItem(0, 60, 60, randomRectHeight);
        rectangle->setBrush(QColor(255,20,147));
        rectangle->setPen(QPen(Qt::white));
        spikesGroup->addToGroup(rectangle);
        spikesGroup->addToGroup(singlePolygon); // adds single spike polygon to the group
    }

    addToGroup(spikesGroup); // adds spikes group to the enemy

    // sets position for the enemy
    setPos(1600, 610 - randomRectHeight);

    // sets up a timer to move the enemy
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(move()));
    timer->start(30);

    // initialize death sound and audio
    death_sound = new QMediaPlayer();
    death_audio = new QAudioOutput();
    death_sound->setSource(QUrl("qrc:/Sound/death_sound.mp3"));
    death_sound->setAudioOutput(death_audio);
    death_audio->setVolume(50);



    ////checking game over sequence based on how many eneimes have been continously created


    //condition for level 2 completion
    if(++continous_enemies<30&&level_base::level==2)
    {
        continous_enemies++;
        qDebug()<<continous_enemies;
    }

    else if(continous_enemies>30&&level_base::level==2)
    {
        continous_enemies=0;
        level2::level_complete();
    }

    //condition for level 3 completion

    if(++continous_enemies<150&&level_base::level==3)
    {
        continous_enemies++;
        qDebug()<<continous_enemies;
    }

    else if(continous_enemies>150&&level_base::level==3)
    {
        continous_enemies=0;
        level3::level_complete();
    }

}

void Enemy::move() {
    setPos(x() - 15, y()); // move the enemy hoirzontally
    QList<QGraphicsItem*> colliding_items = collidingItems(); // checks for collisions

    // removes enemy if it moves out of the scene
    if (x() + boundingRect().width() < boundingRect().width()) {
        scene()->removeItem(this);
        enemies.removeOne(this);
        delete this;
        return;
    }

    // checks for collisions with the player
    for (int i = 0; i < colliding_items.size(); ++i) {
        if (typeid(*(colliding_items[i])) == typeid(m_player)) {
            death_sound->play();
            death_sound->play();
            m_player.setPosition(300,500); // resets player position
            m_player.numOfAttempts++; // increase player attempts count
            m_player.showAttempts();
            player_hit();

            return;
        }
    }
}

void Enemy::player_hit()
{

    // stops level music if playing and restarts after a delay
    if (level2::level2_music && level2::level2_music->isPlaying() == QMediaPlayer::PlayingState) {
        level2::level2_music->stop(); // Stop the music
        QTimer::singleShot(1000, []() {
            level2::level2_music->play(); // Play the music after a delay of 1000 milliseconds (1 second)
        });
    }

    if (level3::level3_music && level3::level3_music->isPlaying() == QMediaPlayer::PlayingState) {
        level3::level3_music->stop(); // Stop the music
        QTimer::singleShot(1000, []() {
            level3::level3_music->play(); // Play the music after a delay of 1000 milliseconds (1 second)
        });
    }

    if (level4::level4_music && level4::level4_music->isPlaying() == QMediaPlayer::PlayingState) {
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

    continous_enemies=0; // resets

    // removes and deletes enemies
    foreach(Enemy*enemy, enemies){
        if(enemy->scene())
            enemy->scene()->removeItem(enemy);
    delete enemy;
    }

    enemies.clear(); // clears the list of enemies

    // removes and deletes coins
    foreach(coin*Coin,coin::coins){
    if(Coin->scene())
            Coin->scene()->removeItem(Coin);
        delete Coin;
    }
    coin::coins.clear(); // clears the list of coins

    // removes and deletes platforms
    foreach(Platform*platform, Platform::platforms){
        if(platform->scene())
            platform->scene()->removeItem(platform);
        delete platform;
    }

    Platform::platforms.clear(); // clears the list of platforms


}

