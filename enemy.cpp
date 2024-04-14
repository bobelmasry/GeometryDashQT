#include "enemy.h"
#include "player.h"
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QPolygonF>
#include <QApplication>

Enemy::Enemy(Player &player) : m_player(player) {
    int randomX = QRandomGenerator::global()->bounded(1000, 1600);
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

    setPos(randomX, 610 - randomRectHeight);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(move()));
    timer->start(30);

    death_sound = new QMediaPlayer();
    death_audio = new QAudioOutput();
    death_sound->setSource(QUrl("qrc:/Sound/death_sound.mp3"));
    death_sound->setAudioOutput(death_audio);
    death_audio->setVolume(50);
}

void Enemy::move() {
    setPos(x() - 11.5, y());
    QList<QGraphicsItem*> colliding_items = collidingItems();

    for (int i = 0; i < colliding_items.size(); ++i) {
        if (typeid(*(colliding_items[i])) == typeid(m_player)) {
            death_sound->play();
            death_sound->play();
            m_player.setPosition(50,500);
            m_player.numOfAttempts++;
            m_player.showAttempts();
            delete this;
            return;
        }
    }
}
