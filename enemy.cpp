#include "enemy.h"
#include "player.h"
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QPolygonF>
#include "level1.h"
#include <coin.h>
#include <QApplication>
#include "platform.h"

QList<Enemy*>Enemy::enemies;

int Enemy::continous_enemies=0;

Enemy::Enemy(Player &player) : m_player(player) {

    enemies.append(this);


    // hasRect == 0 means has rectangle
    int hasRect = QRandomGenerator::global()->bounded(0, 3);
    int randomRectHeight = QRandomGenerator::global()->bounded(30, 45);
    int numOfSpikes = QRandomGenerator::global()->bounded(0, 5);

    if (hasRect > 0) {
        randomRectHeight = 0;
    }

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
        QGraphicsRectItem *rectangle = new QGraphicsRectItem(0, 60, 60, randomRectHeight);
        rectangle->setBrush(QColor(255,20,147));
        rectangle->setPen(QPen(Qt::white));
        spikesGroup->addToGroup(rectangle);
        spikesGroup->addToGroup(singlePolygon);
    }

    addToGroup(spikesGroup);

    setPos(1600, 610 - randomRectHeight);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(move()));
    timer->start(30);

    death_sound = new QMediaPlayer();
    death_audio = new QAudioOutput();
    death_sound->setSource(QUrl("qrc:/Sound/death_sound.mp3"));
    death_sound->setAudioOutput(death_audio);
    death_audio->setVolume(50);



    ////checking game over sequence based on how many eneimes have been continously created

    if(++continous_enemies<80)
    {
        continous_enemies++;
        qDebug()<<continous_enemies;
    }

    else if(continous_enemies>80)
    {
        level1::level_complete();
    }

}

void Enemy::move() {
    setPos(x() - 15, y()); //change speed of spikes ya 7obal
    QList<QGraphicsItem*> colliding_items = collidingItems();

    if (x() + boundingRect().width() < boundingRect().width()) {
        scene()->removeItem(this);
        enemies.removeOne(this);
        delete this;
        return;
    }

    for (int i = 0; i < colliding_items.size(); ++i) {
        if (typeid(*(colliding_items[i])) == typeid(m_player)) {
            death_sound->play();
            death_sound->play();
            m_player.setPosition(300,500);
            m_player.numOfAttempts++;
            m_player.showAttempts();
            player_hit();

            return;
        }
    }
}

void Enemy::player_hit()
{
    QMediaPlayer* level1_music = level1::getLevel1Music();
    if (level1_music) {
        level1_music->stop(); // Stop the music
        QTimer::singleShot(1000, [level1_music]() {
            level1_music->play(); // Play the music after a delay of 1000 milliseconds (1 second)
        });

    }

    continous_enemies=0;

    foreach(Enemy*enemy, enemies){
        if(enemy->scene())
            enemy->scene()->removeItem(enemy);
    delete enemy;
    }

    enemies.clear();

    foreach(coin*Coin,coin::coins){
    if(Coin->scene())
            Coin->scene()->removeItem(Coin);
        delete Coin;
    }
    coin::coins.clear();

    foreach(Platform*platform, Platform::platforms){
        if(platform->scene())
            platform->scene()->removeItem(platform);
        delete platform;
    }

    Platform::platforms.clear();


}

