#include "platform.h"
#include "player.h"
#include <QGraphicsScene>
#include <QTimer>
#include <QMediaPlayer>
#include <QGraphicsRectItem>
#include <QObject>


QList<Platform*>Platform::platforms;

Platform::Platform(qreal x, qreal y, qreal width, qreal height, Player *player)
    : QGraphicsRectItem(x, y, width, height), m_player(player)
{
    setBrush(Qt::black);
    setPen(QPen(Qt::white));
    platforms.append(this);

    QTimer *movement_timer = new QTimer;
    QObject::connect(movement_timer, SIGNAL(timeout()), this, SLOT(move()));
    movement_timer->start(30);
}




///////ask abdallah for this
void Platform::move()
{
    QList<QGraphicsItem*> colliding_items = collidingItems();
    setPos(x() - 15, y());

    for (int i = 0; i < colliding_items.size(); ++i)
    {
        if (typeid(*(colliding_items[i])) == typeid(Player)) {
            Player *player = dynamic_cast<Player*>(colliding_items[i]);

            // Get the positions of the platform and player
            QPointF platformPos = pos();
            QPointF playerPos = player->pos();

            // Get the size of the platform and player
            qreal platformWidth = boundingRect().width();
            qreal platformHeight = boundingRect().height();
            qreal playerWidth = player->boundingRect().width();
            qreal playerHeight = player->boundingRect().height();

            // Check if the player's bottom is above or at the same level as the top of the platform
            if (playerPos.y() + playerHeight <= platformPos.y() &&
                playerPos.x() + playerWidth >= platformPos.x() &&
                playerPos.x() <= platformPos.x() + platformWidth) {
                qDebug() << "Collision detected with player from top side";
                // Handle collision with top side
            }

            // Remove the platform from the scene
            scene()->removeItem(this);
            return;
        }
    }
}







Platform::~Platform()
{

}
