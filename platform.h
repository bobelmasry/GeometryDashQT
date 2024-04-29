#ifndef PLATFORM_H
#define PLATFORM_H

#include <QGraphicsRectItem>
#include "player.h"
#include <QMediaPlayer>


class Platform : public QGraphicsRectItem
{
public:
    Platform(qreal x, qreal y, qreal width, qreal height, Player *player);
    void move();



private:
    Player *m_player; // Pointer to the player object
    QMediaPlayer* death_sound = nullptr;
    QAudioOutput* death_audio = nullptr;



};

#endif // PLATFORM_H
