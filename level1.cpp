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

    QGraphicsView *view = new QGraphicsView(scene);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QPixmap backgroundImage("://images/background.png");
    backgroundImage = backgroundImage.scaled(scene->width(), scene->height());
    QGraphicsPixmapItem *background = new QGraphicsPixmapItem(backgroundImage);
    scene->addItem(background);
    background->setPos(0, 0);

    QGraphicsRectItem *ground = new QGraphicsRectItem(0, scene->height() - 30, scene->width(), 30);
    ground->setBrush(Qt::red);
    scene->addItem(ground);

    Player *player = new Player(scene);
    scene->addItem(player);
    player->setPos(50,700);

    view->setFocus();

    QTimer *time2 = new QTimer();
    QObject::connect(time2, SIGNAL(timeout()), player, SLOT(advance()));
    time2->start(50);

    QTimer *time = new QTimer();
    QObject::connect(time, SIGNAL(timeout()), player, SLOT(createEnemy()));
    QObject::connect(time, SIGNAL(timeout()), player, SLOT(createCoin()));
    time->start(2000);

    //code to play sound when clicking on level
    start_level_audio->setSource(QUrl("qrc:/Sound/start_level_audio.mp3"));
    start_level_audio->setAudioOutput(start_level);
    start_level->setVolume(50);
    start_level_audio->play();

    view->showFullScreen();


}

level1::~level1() {}
