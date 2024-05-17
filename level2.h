#ifndef LEVEL2_H
#define LEVEL2_H

#include "level1.h"
#include "qgraphicsscene.h"
#include <QDialog>
#include <QMediaPlayer>
#include <QAudioOutput>

namespace Ui
{
class level2;
}

class level2: public QDialog
{
    Q_OBJECT

public:
    level2();
    ~level2();
    static void level_complete();
    void elapsed_timer_creator(QGraphicsScene *scene);
    void play_music();
    static QMediaPlayer*level2_music;
private:

    QAudioOutput* start_level = new QAudioOutput();
    QMediaPlayer* start_level_audio = new QMediaPlayer();


    QAudioOutput* base_after_base= new QAudioOutput();


    static QGraphicsScene* scene;
    static QGraphicsView* view;

    static QTimer* enemy_timer;
    static QTimer* coin_timer;
    static QTimer* platform_timer;
    static QTimer* player_timer;
    //static QTimer* elapsed_time;

    void set_level(QGraphicsScene *scene,QGraphicsView *view) ;
};


#endif // LEVEL2_H
