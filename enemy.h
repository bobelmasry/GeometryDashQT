#ifndef ENEMY_H
#define ENEMY_H

#include <QGraphicsItemGroup>
#include <QObject>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QRandomGenerator>
#include <QTimer>

class Enemy : public QObject, public QGraphicsItemGroup
{
    Q_OBJECT
public:
    Enemy();
public slots:
    void move();
private:
    QMediaPlayer* death_sound = nullptr;
    QAudioOutput* death_audio = nullptr;
};

#endif // ENEMY_H
