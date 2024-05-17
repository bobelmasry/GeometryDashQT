#ifndef LEVEL3_H
#define LEVEL3_H

#include "qgraphicsscene.h"
#include <QDialog>
#include <QMediaPlayer>
#include <QAudioOutput>
#include "level1.h"


namespace Ui
{
class level3;
}

class level3: public QDialog
{
    Q_OBJECT

public:
    level3();
    ~level3();
    static void level_complete();
    void elapsed_timer_creator(QGraphicsScene *scene);
    void play_music();
    static QMediaPlayer*level3_music;
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


#endif // LEVEL3_H
