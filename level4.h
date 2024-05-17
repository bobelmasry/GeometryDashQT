#ifndef LEVEL4_H
#define LEVEL4_H

#include "level_base.h"
#include "qgraphicsscene.h"
#include <QDialog>
#include <QMediaPlayer>
#include <QAudioOutput>


namespace Ui
{
class level4;
}

class level4: public QDialog, public level_base
{
    Q_OBJECT

public:
    level4();
    ~level4();
    static void level_complete();
    void elapsed_timer_creator(QGraphicsScene *scene);
    void play_music();
    static QMediaPlayer*level4_music;
private:

    QAudioOutput* start_level = new QAudioOutput();
    QMediaPlayer* start_level_audio = new QMediaPlayer();


    static QGraphicsScene* scene;
    static QGraphicsView* view;

    static QTimer* enemy_timer;
    static QTimer* coin_timer;
    static QTimer* platform_timer;
    static QTimer* player_timer;

    QAudioOutput* electroman= new QAudioOutput();

    void set_level(QGraphicsScene *scene,QGraphicsView *view) ;
};


#endif // LEVEL3_H
