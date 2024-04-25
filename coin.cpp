#include "coin.h"
#include <QGraphicsScene>
#include <QTimer>
#include <QDebug>
#include "player.h"
#include <QGraphicsPixmapItem>

QList<coin*>coin::coins;

coin::coin() {

    coins.append(this);

    setPixmap(QPixmap("://images/coin.png").scaled(100,100));

    int randomX = QRandomGenerator::global()->bounded(600, 800 - pixmap().width());
    int randomY = QRandomGenerator::global()->bounded(400, 600);

    // makes sure that coin isn't colliding with any enemies on spawn
    QList<QGraphicsItem*> colliding_items;
    do {
        setPos(randomX+1300, 500 );
        colliding_items = collidingItems();
        if (!colliding_items.isEmpty()) {
            randomX = QRandomGenerator::global()->bounded(400, 600 - pixmap().width());
            randomY = QRandomGenerator::global()->bounded(400, 600);
        }
    } while (!colliding_items.isEmpty()); // repeats until no collisions are detected

    QTimer *timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(move()));
    timer->start(30);
}


void coin::move() {
    setPos(x() - 15, y());
    QList<QGraphicsItem*> colliding_items = collidingItems();

    if (x() + pixmap().width() < pixmap().width()) {
        scene()->removeItem(this);
        coins.removeOne(this);
        delete this;
        return;
    }

    for (int i = 0; i < colliding_items.size(); ++i) {
        if (typeid(*(colliding_items[i])) == typeid(Player)) {
            Player *player = dynamic_cast<Player*>(colliding_items[i]);
            player->increase();
                scene()->removeItem(this);

            coins.removeOne(this);
                delete this;
                return;
        }
    }
}
