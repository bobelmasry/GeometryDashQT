#include "coin.h"
#include <QGraphicsScene>
#include <QTimer>
#include <QDebug>
#include "player.h"
#include <QGraphicsPixmapItem>

coin::coin() {
    setPixmap(QPixmap("://images/coin.png").scaled(100,100));

    int randomX = QRandomGenerator::global()->bounded(400, 600 - pixmap().width());
    int randomY = QRandomGenerator::global()->bounded(0, 600 - pixmap().height());

    // makes sure that coin isn't colliding with any enemies on spawn
    QList<QGraphicsItem*> colliding_items;
    do {
        setPos(randomX, randomY);
        colliding_items = collidingItems();
        if (!colliding_items.isEmpty()) {
            randomX = QRandomGenerator::global()->bounded(400, 600 - pixmap().width());
            randomY = QRandomGenerator::global()->bounded(0, 600 - pixmap().height());
        }
    } while (!colliding_items.isEmpty()); // repeats until no collisions are detected

    QTimer *timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(move()));
    timer->start(50);
}


void coin::move() {
    setPos(x() - 5, y());
    QList<QGraphicsItem*> colliding_items = collidingItems();


    if (y() + pixmap().height() > 800) {
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
            player->increase();
                scene()->removeItem(this);

                delete this;
                return;
        }
    }
}
