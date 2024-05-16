#include "platform.h"
#include "player.h"
#include <QGraphicsScene>
#include <QTimer>
#include <QList>

QList<Platform *> Platform::platforms;

Platform::Platform(qreal x, qreal y, qreal width, qreal height, Player *player)
    : QGraphicsRectItem(x, y, width, height)
    , m_player(player)
{
    setBrush(Qt::black);
    setPen(QPen(Qt::white));
    platforms.append(this);

    QTimer *movement_timer = new QTimer(this);
    connect(movement_timer, SIGNAL(timeout()), this, SLOT(move()));
    movement_timer->start(30);
}

void Platform::move()
{
    QList<QGraphicsItem *> colliding_items = collidingItems();
    setPos(x() - 15, y());

    for (int i = 0; i < colliding_items.size(); ++i) {
        if (typeid(*(colliding_items[i])) == typeid(Player)) {
            Player *player = dynamic_cast<Player *>(colliding_items[i]);

            // Check if the player is jumping
            if (player->isInAir()) {
                // Increase the jump by adding a higher positive vertical velocity
                player->setYVelocity(-player->getYVelocity() ); // Adjust the bounce strength as needed
                player->setYVelocity(player->getYVelocity() -30); // Increase the upward velocity
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
