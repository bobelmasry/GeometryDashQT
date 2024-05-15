#include "platform.h"
#include <QGraphicsScene>
#include <QTimer>
#include <QMediaPlayer>
#include "level1.h"



Platform::Platform(qreal x, qreal y, qreal width, qreal height, Player *player)
    : QGraphicsRectItem(x, y, width, height), m_player(player) {
    setBrush(Qt::blue);

    death_sound = new QMediaPlayer();
    death_audio = new QAudioOutput();
    death_sound->setSource(QUrl("qrc:/Sound/death_sound.mp3"));
    death_sound->setAudioOutput(death_audio);
    death_audio->setVolume(50);
}

void Platform::move() {
    setPos(x() - 15, y());

    // Check if the platform is out of bounds
    if (x() + boundingRect().width() < boundingRect().width()) {
        // Remove the platform from the scene and delete it
        scene()->removeItem(this);
        delete this;
        return;
    }

    if (collidesWithItem(m_player)) {
        // If the player collides with the platform, delete the player object

        death_sound->play();
        death_sound->play();
        m_player->numOfAttempts++;
        m_player->showAttempts();
        scene()->removeItem(m_player);
        delete m_player;
    }

}
