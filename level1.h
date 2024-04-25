#ifndef LEVEL1_H
#define LEVEL1_H

#include "enemy.h"
#include <QDialog>
#include <QMediaPlayer>
#include <QAudioOutput>

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
    void setLevel1Music(QMediaPlayer *music);


private:
    QAudioOutput* start_level = new QAudioOutput();
    QMediaPlayer* start_level_audio = new QMediaPlayer();
    QAudioOutput* steromadness= new QAudioOutput();
    static QMediaPlayer* level1_music;



};

#endif // LEVEL1_H
