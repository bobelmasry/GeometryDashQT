#include "enemy.h"
#include <QGraphicsScene>
#include <QTimer>
#include <QDebug>
#include "player.h"

Enemy::Enemy() {
    QPolygonF polygon;
    polygon << QPointF(0, 30) << QPointF(15, 0) << QPointF(30, 30);

    setBrush(QColor(255,20,147));
    setPen(QPen(Qt::white));

    setPolygon(polygon);

    setPos(800, 640);

    QTimer *timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(move()));
    timer->start(50);

    death_sound = new QMediaPlayer();
    death_audio = new QAudioOutput();
    death_sound->setSource(QUrl("qrc:/Sound/death_sound.mp3"));
    death_sound->setAudioOutput(death_audio);
    death_audio->setVolume(50);
}


void Enemy::move() {
    setPos(x() - 15, y());
    QList<QGraphicsItem*> colliding_items = collidingItems();

    if (y() + polygon().boundingRect().height() > 800) {
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
            Player *player = dynamic_cast<Player*>(colliding_items[i]);
            player->decrease();
            if (player) {
                scene()->removeItem(colliding_items[i]);
                delete colliding_items[i];
                delete this;
                return;
            }
        }
    }
}
