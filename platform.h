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
    static QList<Platform *> platforms;
    static int continious_plats;


public slots:
    void move();
    void player_hit();


signals:
    void left_collision(QGraphicsItem *player);
    void plat_player_collision();

private:
    QMediaPlayer *death_sound;
    QAudioOutput *death_audio;


};

#endif // PLATFORM_H
