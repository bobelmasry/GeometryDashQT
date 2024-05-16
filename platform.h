#ifndef PLATFORM_H
#define PLATFORM_H

#include <QGraphicsItemGroup>
#include <QGraphicsRectItem>
#include <QMediaPlayer>
#include <QObject>
#include <QRandomGenerator>

class Player;
class Platform : public QObject, public QGraphicsItemGroup
{
    Q_OBJECT
public:
    Platform();
    ~Platform();


public slots:
    void move();

signals:
    void left_collision(QGraphicsItem *player);
    void plat_player_collision();

private:
    QMediaPlayer *death_sound;
    QAudioOutput *death_audio;
    static QList<Platform *> platforms;

};

#endif // PLATFORM_H
