#ifndef LEVEL1_H
#define LEVEL1_H

#include "level.h"
#include "player.h"
#include "qtimer.h"
#include <QDialog>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QGraphicsScene>
#include <QGraphicsView>


namespace Ui {
class level1;
}

class level1 : public QDialog
{
    Q_OBJECT

public:
    level1();
    ~level1();
    static QMediaPlayer* getLevel1Music();
    static void setLevel1Music(QMediaPlayer *music);
    QTimer *gametimer=new QTimer();
    static void level_complete();
    void play_music();
    static int elapsedSec;
    void elapsed_timer_creator(QGraphicsScene *scene);
    void update_timer(QGraphicsTextItem *elapsedTimeText, int &elapsedSec);
    void set_level(QGraphicsScene *scene,QGraphicsView *view);


private:
    Ui::level1 *ui;
    QAudioOutput* start_level = new QAudioOutput();
    QMediaPlayer* start_level_audio = new QMediaPlayer();
    QAudioOutput* steromadness= new QAudioOutput();

    static QMediaPlayer* level1_music;

    static QGraphicsScene* scene;
    static QGraphicsView* view;

    static QTimer* enemy_timer;
    static QTimer* coin_timer;
    static QTimer *elapsed_time;



};

#endif // LEVEL1_H
