#ifndef ENEMY_H
#define ENEMY_H

#include <QGraphicsItemGroup>
#include <QObject>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QRandomGenerator>
#include <QTimer>
#include "player.h"

class Enemy : public QObject, public QGraphicsItemGroup
{
    Q_OBJECT
public:
    Enemy(Player& player);
public slots:
    void move();
    void player_hit();
private:
    QMediaPlayer* death_sound = nullptr;
    QAudioOutput* death_audio = nullptr;
    Player& m_player;
    static QList<Enemy*> enemies;

};

#endif // ENEMY_H
