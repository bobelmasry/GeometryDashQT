#include "level1.h"
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QGraphicsRectItem>
#include "mainwindow.h"
#include "player.h"
#include <QGraphicsPixmapItem>
#include <enemy.h>
#include<coin.h>



QMediaPlayer* level1::level1_music = nullptr;

int level1::gamecounter=0;

level1::level1()
{


    QGraphicsScene *scene = new QGraphicsScene();
    scene->setSceneRect(0, 0, 1560, 870);

    // Initialize and play the start_level audio before level1_music
    start_level_audio->setSource(QUrl("qrc:/Sound/start_level_audio.mp3"));
    start_level_audio->setAudioOutput(start_level);
    start_level->setVolume(50);
    start_level_audio->play();

    // Initialize level1_music
    level1_music = new QMediaPlayer();
    level1_music->setSource(QUrl("qrc:/Sound/level1_music.mp3"));
    level1_music->setAudioOutput(steromadness);
    steromadness->setVolume(50);

    // Delay the level1_music audio
    QTimer::singleShot(1000, this, []() {
        if (level1_music) {
            level1_music->play();
        }
    });


    QGraphicsView *view = new QGraphicsView(scene);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QPixmap backgroundImage(":/images/level1_background.png");
    backgroundImage = backgroundImage.scaled(scene->width(), scene->height());
    QGraphicsPixmapItem *background = new QGraphicsPixmapItem(backgroundImage);
    scene->addItem(background);
    background->setPos(0, 0);

    QPixmap level1_floor_image(":/images/level1_floor.png");
    level1_floor_image=level1_floor_image.scaled(scene->width(),400);
    QGraphicsPixmapItem*level1_floor=new QGraphicsPixmapItem(level1_floor_image);
    level1_floor->setPos(0,scene->height()-200);
    scene->addItem(level1_floor);

    Player *player = new Player(scene);
    scene->addItem(player);
    player->setPos(300,400);
    view->setFocus();

    view->showFullScreen();

    QTimer *time = new QTimer();
    QObject::connect(time, SIGNAL(timeout()), player, SLOT(createEnemy()));
    time->start(1000);

    QTimer *time2 = new QTimer();
    QObject::connect(time2, SIGNAL(timeout()), player, SLOT(advance()));
    time2->start(50);

    QTimer *time3 = new QTimer();
    QObject::connect(time3, SIGNAL(timeout()), player, SLOT(createCoin()));
    time3->start(1500);

     QTimer *timeCheck = new QTimer();
     timeCheck->start(1000);


    QObject::connect(timeCheck, &QTimer::timeout, [ time, time3, timeCheck, view, scene]() {

        QGraphicsTextItem *elapsedTimeText = new QGraphicsTextItem();
        QFont font;
        font.setPointSize(14);
        elapsedTimeText->setFont(font);
        elapsedTimeText->setDefaultTextColor(Qt::white);
        scene->addItem(elapsedTimeText);
        int textWidth = 200; // Adjust text width according to your scene
        elapsedTimeText->setPos(scene->width() - textWidth - 10+1000, 10); // Adjust
        QObject::connect(time, &QTimer::timeout, [elapsedTimeText]() {
            gamecounter++;
            elapsedTimeText->setPlainText(QString("Elapsed Time: %1 seconds").arg(gamecounter));
        });

        if (gamecounter >= 400)

        {
            time->stop(); // Stop creating enemies
            time3->stop(); // Stop creating coins
            timeCheck->stop();
            gamecounter=0;

            QTimer::singleShot(3000, [&]()
            {
                QGraphicsTextItem *gameOverText = new QGraphicsTextItem("Level Complete!");
                QFont font;
                font.setPointSize(30);
                gameOverText->setFont(font);
                gameOverText->setDefaultTextColor(Qt::white);
                gameOverText->setPos((scene->width() - gameOverText->boundingRect().width()) / 2,
                                     (scene->height() - gameOverText->boundingRect().height()) / 2);
                scene->addItem(gameOverText);

                // Close the window after 3 seconds
                QTimer::singleShot(3000, view, &QGraphicsView::close);
                timeCheck->stop();
                QTimer::singleShot(3000, [&]() {
                    MainWindow *windowObj = new MainWindow();
                    windowObj->show();
                    level1_music->stop();
                });
            });
        }

    });


}


QMediaPlayer* level1::getLevel1Music()
{
    return level1_music;
}

level1::~level1() {}

void level1::level1_complete()
{

}
