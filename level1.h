#ifndef LEVEL1_H
#define LEVEL1_H

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

private:
    QAudioOutput* start_level = new QAudioOutput();
    QMediaPlayer* start_level_audio = new QMediaPlayer();
    QAudioOutput* steromadness= new QAudioOutput();
    QMediaPlayer* level1_music= new QMediaPlayer();



};

#endif // LEVEL1_H
