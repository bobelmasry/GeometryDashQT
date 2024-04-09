#include "enemy.h"
#include "player.h"
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QPolygonF>
#include <QApplication>

Enemy::Enemy() {
    QPolygonF polygon;
    polygon << QPointF(0, 60) << QPointF(30, 0) << QPointF(60, 60);
    int randomX = QRandomGenerator::global()->bounded(1000, 1600);
    int randomRectHeight = QRandomGenerator::global()->bounded(10, 80);

    // Create the polygon and set its brush and pen
    QGraphicsPolygonItem *polygonItem = new QGraphicsPolygonItem(polygon);
    polygonItem->setBrush(QColor(255,20,147));
    polygonItem->setPen(QPen(Qt::white));
    addToGroup(polygonItem);

    // Create the rectangle and set its brush and pen
    QGraphicsRectItem *rectangle = new QGraphicsRectItem(0, 60, 60, randomRectHeight);
    rectangle->setBrush(QColor(255,20,147));
    rectangle->setPen(QPen(Qt::white));
    addToGroup(rectangle);

    setPos(randomX, 610 - randomRectHeight);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(move()));
    timer->start(50);

    death_sound = new QMediaPlayer();
    death_audio = new QAudioOutput();
    death_sound->setSource(QUrl("qrc:/Sound/death_sound.mp3"));
    death_sound->setAudioOutput(death_audio);
    death_audio->setVolume(50);
}


void Enemy::move() {
    setPos(x() - 30, y());
    QList<QGraphicsItem*> colliding_items = collidingItems();

    if (y() + boundingRect().height() > 800) {
        QList<QGraphicsItem*> scene_items = scene()->items();
        for (int j = 0; j < scene_items.size(); ++j) {
            Player *player = dynamic_cast<Player*>(scene_items[j]);
            if (player) {
                player->decrease();
                break;
            }
        }
        scene()->removeItem(this);
        delete this;
        return;
    }

    for (int i = 0; i < colliding_items.size(); ++i) {
        if (typeid(*(colliding_items[i])) == typeid(Player)) {
            death_sound->play();
            death_sound->play();
            Player *player = dynamic_cast<Player*>(colliding_items[i]);
            player->decrease();
            if (player) {
                scene()->removeItem(colliding_items[i]);
                delete colliding_items[i];
                delete this;
                QApplication::quit();
                return;
            }
        }
    }
}
