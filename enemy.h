#ifndef ENEMY_H
#define ENEMY_H

#include <QGraphicsPolygonItem>
#include <QObject>
#include <QTimer>
#include <QRandomGenerator>
#include <QMediaPlayer>
#include <QAudioOutput>

class Enemy : public QObject, public QGraphicsPolygonItem
{
    Q_OBJECT
public:
    void extracted();
    Enemy();
public slots:
    void move();
private:
    QMediaPlayer* death_sound = new QMediaPlayer;
    QAudioOutput* death_audio = new QAudioOutput;
};

#endif // ENEMY_H
