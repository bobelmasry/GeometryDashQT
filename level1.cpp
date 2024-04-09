#include "level1.h"
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QGraphicsRectItem>
#include "player.h"
#include <QGraphicsPixmapItem>

level1::level1()
{
    QGraphicsScene *scene = new QGraphicsScene();
    scene->setSceneRect(0, 0, 1560, 870);

    // Initialize and play the start_level audio before level1_music
    start_level_audio->setSource(QUrl("qrc:/Sound/start_level_audio.mp3"));
    start_level_audio->setAudioOutput(start_level);
    start_level->setVolume(50);
    start_level_audio->play();

    // Delay the level1_music audio
    QTimer::singleShot(1000, this, [this]() {
        level1_music->setSource(QUrl("qrc:/Sound/level1_music.mp3"));
        level1_music->setAudioOutput(steromadness);
        steromadness->setVolume(50);
        level1_music->play();
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

    QTimer *time2 = new QTimer();
    QObject::connect(time2, SIGNAL(timeout()), player, SLOT(advance()));
    time2->start(50);

    QTimer *time = new QTimer();
    QObject::connect(time, SIGNAL(timeout()), player, SLOT(createEnemy()));
    time->start(1500);

    QTimer *time3 = new QTimer();
    QObject::connect(time, SIGNAL(timeout()), player, SLOT(createCoin()));
    time3->start(1500);

    view->showFullScreen();
}

level1::~level1() {}
