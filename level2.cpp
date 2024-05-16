#include "level2.h"
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QGraphicsRectItem>
#include "player.h"
#include "qobjectdefs.h"
#include <QGraphicsPixmapItem>

QGraphicsScene*level2::scene;
QGraphicsView*level2::view;
QTimer*level2::coin_timer;
QTimer*level2::platform_timer;
QTimer*level2::enemy_timer;

level2::level2()
{
    scene = new QGraphicsScene();

    view = new QGraphicsView(scene);

    set_level(scene,view);

    Player *player = new Player(scene);
    scene->addItem(player);
    player->setPos(300,400);
    view->setFocus();

    QTimer *player_timer = new QTimer();
    QObject::connect(player_timer, SIGNAL(timeout()), player, SLOT(advance()));
    player_timer->start(50);

    enemy_timer = new QTimer();
    coin_timer= new QTimer();
    platform_timer= new QTimer();

   /* QObject::connect(enemy_timer, SIGNAL(timeout()), player, SLOT(createEnemy()));
    QObject::connect(coin_timer, SIGNAL(timeout()), player, SLOT(createCoin()));*/
    QObject::connect(platform_timer, SIGNAL(timeout()), player, SLOT(createPlatform()));

    enemy_timer->start(1500);
    platform_timer->start(1000);
    coin_timer->start(1000);




    //code to play sound when clicking on level
    start_level_audio->setSource(QUrl("qrc:/Sound/start_level_audio.mp3"));
    start_level_audio->setAudioOutput(start_level);
    start_level->setVolume(50);
    start_level_audio->play();

    view->showFullScreen();


}

void level2::set_level(QGraphicsScene*scene,QGraphicsView*view)
{
    scene->setSceneRect(0, 0, 1560, 870);

    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QPixmap backgroundImage(":/images/level2_background.png");
    backgroundImage = backgroundImage.scaled(scene->width(), scene->height());
    QGraphicsPixmapItem *background = new QGraphicsPixmapItem(backgroundImage);
    scene->addItem(background);
    background->setPos(0, 0);

    QPixmap level1_floor_image(":/images/level2_floor.png");
    level1_floor_image=level1_floor_image.scaled(scene->width(),400);
    QGraphicsPixmapItem*level1_floor=new QGraphicsPixmapItem(level1_floor_image);
    level1_floor->setPos(0,scene->height()-200);
    scene->addItem(level1_floor);

}

level2::~level2() {}
