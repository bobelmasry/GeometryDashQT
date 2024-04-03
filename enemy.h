#ifndef ENEMY_H
#define ENEMY_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QTimer>
#include <QRandomGenerator>
#include <QMediaPlayer>
#include <QAudioOutput>

class Enemy : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    Enemy();
public slots:
    void move();
private:
    QMediaPlayer*death_sound = new QMediaPlayer;
    QAudioOutput*death_audio = new QAudioOutput;
};

#endif // ENEMY_H


