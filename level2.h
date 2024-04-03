#ifndef LEVEL2_H
#define LEVEL2_H

#include <QDialog>
#include <QMediaPlayer>
#include <QAudioOutput>

namespace Ui {
class level2;
}

class level2 : public QDialog
{
    Q_OBJECT

public:
    level2();
    ~level2();
private:
    QAudioOutput* start_level = new QAudioOutput();
    QMediaPlayer* start_level_audio = new QMediaPlayer();
};

#endif // LEVEL2_H
