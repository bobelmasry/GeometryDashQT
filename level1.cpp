#include "level1.h"
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QGraphicsRectItem>
#include "mainwindow.h"
#include "player.h"
#include "qobjectdefs.h"
#include <QGraphicsPixmapItem>
#include <enemy.h>
#include <coin.h>
#include <QDateTime>
#include "level_base.h"

// declare static member variables
QMediaPlayer* level1::level1_music = nullptr;

QGraphicsScene* level1::scene;
QGraphicsView* level1::view;
QTimer* level1::enemy_timer;
QTimer* level1::coin_timer;
QTimer* level1::elapsed_time;
QTimer* level1::platform_timer;

// constructor
level1::level1()
{
    // set the level to 1
    level_base::level = 1;

    // create a new graphics scene
    scene = new QGraphicsScene();

    // create a new graphics view with the scene
    view = new QGraphicsView(scene);

    // set up the level with the scene and view
    set_level(scene, view);

    // play the background music
    play_music();

    // create a new player object and add it to the scene
    Player *player = new Player(scene);
    scene->addItem(player);

    // set the player's position
    player->setPos(300, 400);

    // set the view to focus on the player
    view->setFocus();

    // show the view in full screen mode
    view->showFullScreen();

    // create a timer to generate coins periodically
    coin_timer = new QTimer();
    QObject::connect(coin_timer, &QTimer::timeout, player, &Player::createCoin);
    coin_timer->start(1500);

    // create a timer to generate platforms periodically
    platform_timer = new QTimer();
    QObject::connect(platform_timer, &QTimer::timeout, player, &Player::createPlatform);
    platform_timer->start(1500);

    // create a timer to advance the player's state
    QTimer *player_timer = new QTimer();
    QObject::connect(player_timer, &QTimer::timeout, player, &Player::advance);
    player_timer->start(50);

    // create and start the elapsed timer
    elapsed_timer_creator(scene);
}

// function to set up the level
void level1::set_level(QGraphicsScene *scene, QGraphicsView *view)
{
    // set the scene rectangle size
    scene->setSceneRect(0, 0, 1560, 870);

    // disable scroll bars
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // set the background image
    QPixmap backgroundImage(":/images/level1_background.png");
    backgroundImage = backgroundImage.scaled(scene->width(), scene->height());
    QGraphicsPixmapItem *background = new QGraphicsPixmapItem(backgroundImage);
    scene->addItem(background);
    background->setPos(0, 0);

    // set the floor image
    QPixmap level1_floor_image(":/images/level1_floor.png");
    level1_floor_image = level1_floor_image.scaled(scene->width(), 400);
    QGraphicsPixmapItem *level1_floor = new QGraphicsPixmapItem(level1_floor_image);
    level1_floor->setPos(0, scene->height() - 200);
    scene->addItem(level1_floor);
}

// function to update the elapsed time
void level1::update_timer(QGraphicsTextItem *elapsedTimeText, int &elapsedSec)
{
    // increment elapsed seconds and update the display text
    elapsedSec++;
    elapsedTimeText->setPlainText(QString("Elapsed Time: %1 seconds").arg(elapsedSec));
}

// function to handle level completion
void level1::level_complete()
{
    qDebug() << "entered level complete";

    // stop the timers if they are active
    if (coin_timer->isActive())
        coin_timer->stop();
    if (platform_timer->isActive())
        platform_timer->stop();


    qDebug() << "stopped timers";

    // using a single-shot timer to delay actions by 3 seconds
    QTimer::singleShot(3000, []()
                       {
                           // display "level complete" text in the center of the screen
                           QGraphicsTextItem *gameOverText = new QGraphicsTextItem("Level Complete!");
                           QFont font;
                           font.setPointSize(30);
                           gameOverText->setFont(font);
                           gameOverText->setDefaultTextColor(Qt::white);
                           gameOverText->setPos((scene->width() - gameOverText->boundingRect().width()) / 2,
                                                (scene->height() - gameOverText->boundingRect().height()) / 2);
                           scene->addItem(gameOverText);

                           // close the window after another 3 seconds
                           QTimer::singleShot(3000, []()
                                              {
                                                  view->close();
                                                  if (elapsed_time && elapsed_time->isActive())
                                                      elapsed_time->stop();
                                                  MainWindow *windowObj = new MainWindow();
                                                  windowObj->show();
                                                  if (level1_music)
                                                      level1_music->stop();
                                              });
                       });
}

// function to play background music
void level1::play_music()
{
    // initialize and play the start_level audio before level1_music
    start_level_audio->setSource(QUrl("qrc:/Sound/start_level_audio.mp3"));
    start_level_audio->setAudioOutput(start_level);
    start_level->setVolume(50);
    start_level_audio->play();

    // initialize level1_music
    level1_music = new QMediaPlayer();
    level1_music->setSource(QUrl("qrc:/Sound/level1_music.mp3"));
    level1_music->setAudioOutput(steromadness);
    steromadness->setVolume(50);

    // delay the level1_music audio
    QTimer::singleShot(1000, []() {
        if (level1::level1_music) {
            level1::level1_music->play();
        }
    });
}

// function to get the current level music player
QMediaPlayer* level1::getLevel1Music()
{
    return level1_music;
}

// destructor for level1 class
level1::~level1() {}
