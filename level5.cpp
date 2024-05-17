#include "level5.h"
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QGraphicsRectItem>
#include "mainwindow.h"
#include "player.h"
#include "qobjectdefs.h"
#include <QGraphicsPixmapItem>
#include "level_base.h"


// declare static member variables
QGraphicsScene*level5::scene;
QGraphicsView*level5::view;
QTimer*level5::coin_timer;
QTimer*level5::platform_timer;
QTimer*level5::enemy_timer;

QMediaPlayer*level5::level5_music=nullptr;

// constructor
level5::level5()
{
    // set the level to 5
    level_base::level=5;
    // create a new graphics scene
    scene = new QGraphicsScene();

    // create a new graphics view with the scene
    view = new QGraphicsView(scene);

    // set up the level with the scene and view
    set_level(scene,view);

    // play the background music
    play_music();

    // create and start the elapsed timer
    level_base::elapsed_timer_creator(scene);

    // create a new player object and add it to the scene
    Player *player = new Player(scene);
    scene->addItem(player);
    // set the player's position
    player->setPos(300,400);
    // set the view to focus on the player
    view->setFocus();

    // create a timer to advance the player's state
    QTimer *player_timer = new QTimer();
    QObject::connect(player_timer, SIGNAL(timeout()), player, SLOT(advance()));
    player_timer->start(50);

    // create timers for generating enemies, coins, and platforms
    enemy_timer = new QTimer();
    coin_timer= new QTimer();
    platform_timer= new QTimer();

    // connect timers to the respective player functions
    QObject::connect(enemy_timer, SIGNAL(timeout()), player, SLOT(createEnemy()));
    QObject::connect(coin_timer, SIGNAL(timeout()), player, SLOT(createCoin()));
    QObject::connect(platform_timer, SIGNAL(timeout()), player, SLOT(createPlatform()));

    // start the timers with specified intervals
    enemy_timer->start(1500);
    platform_timer->start(800);
    coin_timer->start(5000);

    // show the view in full screen mode
    view->showFullScreen();


}

// function to set up the level
void level5::set_level(QGraphicsScene*scene,QGraphicsView*view)
{
    // set the scene rectangle size
    scene->setSceneRect(0, 0, 1560, 870);


    // disable scroll bars
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // set the background image
    QPixmap backgroundImage(":/images/level5_background.png");
    backgroundImage = backgroundImage.scaled(scene->width(), scene->height());
    QGraphicsPixmapItem *background = new QGraphicsPixmapItem(backgroundImage);
    scene->addItem(background);
    background->setPos(0, 0);

    // set the floor image
    QPixmap level_floor_image(":/images/level5_floor.png");
    level_floor_image=level_floor_image.scaled(scene->width(),400);
    QGraphicsPixmapItem*level1_floor=new QGraphicsPixmapItem(level_floor_image);
    level1_floor->setPos(0,scene->height()-200);
    scene->addItem(level1_floor);


}

// function to play background music
void level5::play_music()
{
    //code to play sound when clicking on level
    start_level_audio->setSource(QUrl("qrc:/Sound/start_level_audio.mp3"));
    start_level_audio->setAudioOutput(start_level);
    start_level->setVolume(50);
    start_level_audio->play();

    // Initialize level1_music
    level5_music = new QMediaPlayer();
    level5_music->setSource(QUrl("qrc:/Sound/level5_music.mp3"));
    level5_music->setAudioOutput(hexagon);
    hexagon->setVolume(50);

    // Delay the level1_music audio
    QTimer::singleShot(1000, this, []() {
        if (level5_music) {
            level5_music->play();
        }
    });

}


// function to handle level completion
void level5::level_complete()
{
    qDebug() << "entered level complete";

    // Stop the timers
    if (coin_timer && coin_timer->isActive())
        coin_timer->stop();
    if (enemy_timer && enemy_timer->isActive())
        enemy_timer->stop();
    if (platform_timer && platform_timer->isActive())
        platform_timer->stop();

    qDebug() << "stopped timers";

    // Ensure scene and view are valid
    if (!scene || !view) {
        qDebug() << "scene or view is null!";
        return;
    }

    // Capture necessary pointers for lambda functions
    QGraphicsScene *local_scene = scene;
    QGraphicsView *local_view = view;

    // Using a single-shot timer to delay actions by 3 seconds
    QTimer::singleShot(3000, [local_scene, local_view]() {
        if (!local_scene || !local_view) {
            qDebug() << "local_scene or local_view is null in singleShot!";
            return;
        }

        QGraphicsTextItem *gameOverText = new QGraphicsTextItem("Level Complete!");
        QFont font;
        font.setPointSize(30);
        gameOverText->setFont(font);
        gameOverText->setDefaultTextColor(Qt::white);
        gameOverText->setPos((local_scene->width() - gameOverText->boundingRect().width()) / 2,
                             (local_scene->height() - gameOverText->boundingRect().height()) / 2);
        local_scene->addItem(gameOverText);

        // Close the window after another 3 seconds
        QTimer::singleShot(3000, [local_view]() {
            if (!local_view) {
                qDebug() << "local_view is null in second singleShot!";
                return;
            }

            local_view->close();

            MainWindow *windowObj = new MainWindow();
            windowObj->show();
            if (level5::level5_music)
                level5::level5_music->stop();
        });
    });
}

level5::~level5() {}
