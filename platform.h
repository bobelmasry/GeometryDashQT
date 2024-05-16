#ifndef PLATFORM_H
#define PLATFORM_H

#include <QGraphicsRectItem>
#include <QObject>
#include <QMediaPlayer>

class Player;
class Platform : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    Platform(qreal x, qreal y, qreal width, qreal height, Player *player);
     ~Platform();

public slots:
    void move();

signals:
    void left_collision(QGraphicsItem* player);
    void plat_player_collision();

private:
    Player *m_player;
    QMediaPlayer *death_sound;
    QAudioOutput *death_audio;
    static QList<Platform*> platforms;
};

#endif // PLATFORM_H
