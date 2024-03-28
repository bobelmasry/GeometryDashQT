#include "enemy.h"
#include <QGraphicsScene>
#include <QTimer>
#include <QDebug>
#include "player.h"
#include <QGraphicsRectItem>

Enemy::Enemy() {
    setBrush(QColor(255,20,147));
    setPen(QPen(Qt::white));

    int heightVar = QRandomGenerator::global()->bounded(50, 100);
    int widthVar = QRandomGenerator::global()->bounded(25, 50);
    setRect(0, 0, widthVar, heightVar);


    int random_number = QRandomGenerator::global()->bounded(100, 600 - heightVar - 30);
    setPos(800, random_number);

    QTimer *timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(move()));
    timer->start(50);
}


void Enemy::move() {
    setPos(x() - 5, y());
    QList<QGraphicsItem*> colliding_items = collidingItems();


    if (y() + rect().height() > 800) {
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
