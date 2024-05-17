#ifndef LEVEL1_H
#define LEVEL1_H

#include "level_base.h"
#include "qtimer.h"
#include <QDialog>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QGraphicsScene>
#include <QGraphicsView>


namespace Ui {
class level1;
}

class level1 : public QDialog, public level_base
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
    void update_timer(QGraphicsTextItem *elapsedTimeText, int &elapsedSec);
    void set_level(QGraphicsScene *scene,QGraphicsView *view);
     static QMediaPlayer* level1_music;


private:
    Ui::level1 *ui;
    QAudioOutput* start_level = new QAudioOutput();
    QMediaPlayer* start_level_audio = new QMediaPlayer();
    QAudioOutput* steromadness= new QAudioOutput();



    static QGraphicsScene* scene;
    static QGraphicsView* view;

    static QTimer* enemy_timer;
    static QTimer* coin_timer;
    static QTimer *elapsed_time;
    static QTimer* platform_timer;



};

#endif // LEVEL1_H
