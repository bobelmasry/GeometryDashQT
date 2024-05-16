#include "platform.h"
#include "player.h"
#include <QGraphicsScene>
#include <QTimer>
#include <QList>

QList<Platform *> Platform::platforms;

Platform::Platform()
{
    platforms.append(this);

    QGraphicsItemGroup *platformGroup = new QGraphicsItemGroup();

    int randomRectHeight = QRandomGenerator::global()->bounded(30, 45);
    QGraphicsRectItem *rectangle = new QGraphicsRectItem(0, 60, 60, randomRectHeight);
    QGraphicsRectItem *square = new QGraphicsRectItem(60, 120, randomRectHeight, 5 + randomRectHeight);
    rectangle->setBrush(QColor(255,20,147));
    rectangle->setPen(QPen(Qt::blue));
    platformGroup->addToGroup(rectangle);
    platformGroup->addToGroup(square);

    setPos(200, 610 - randomRectHeight);

    QTimer *movement_timer = new QTimer(this);
    connect(movement_timer, SIGNAL(timeout()), this, SLOT(move()));
    movement_timer->start(30);
}

void Platform::move()
{
    QList<QGraphicsItem *> colliding_items = collidingItems();
    qDebug() << "Platform Moved.";
    setPos(x() - 15, y());

    for (int i = 0; i < colliding_items.size(); ++i) {
        if (typeid(*(colliding_items[i])) == typeid(Player)) {
            Player *player = dynamic_cast<Player *>(colliding_items[i]);

            // Check if the player is jumping
            if (player->isInAir()) {
                // Increase the jump by adding a higher positive vertical velocity
                player->setYVelocity(player->getYVelocity() -15); // Increase the upward velocity
                // Since jumping is allowed during bouncing, set the player's in_air flag to false
                player->setInAir(false);
            } else {
                // If the player is not jumping, remove the platform
                scene()->removeItem(this);
                delete this;
                return;
            }
        }
    }
}

Platform::~Platform() {}
