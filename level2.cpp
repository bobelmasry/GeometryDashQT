#include "level2.h"
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

QGraphicsScene* level2::scene = nullptr;
QGraphicsView* level2::view = nullptr;
QTimer* level2::coin_timer = nullptr;
QTimer* level2::platform_timer = nullptr;
QTimer* level2::enemy_timer = nullptr;
QMediaPlayer* level2::level2_music = nullptr;

level2::level2()
{
    level_base::level = 2;
    scene = new QGraphicsScene();
    view = new QGraphicsView(scene);

    set_level(scene, view);
    play_music();

    level_base::elapsed_timer_creator(scene);

    Player *player = new Player(scene);
    scene->addItem(player);
    player->setPos(300, 400);
    view->setFocus();

    QTimer *player_timer = new QTimer();
    QObject::connect(player_timer, &QTimer::timeout, player, &Player::advance);
    player_timer->start(50);

    enemy_timer = new QTimer();
    coin_timer = new QTimer();
    platform_timer = new QTimer();

    QObject::connect(enemy_timer, &QTimer::timeout, player, &Player::createEnemy);
    QObject::connect(coin_timer, &QTimer::timeout, player, &Player::createCoin);
    // QObject::connect(platform_timer, &QTimer::timeout, player, &Player::createPlatform);

    enemy_timer->start(1500);
    // platform_timer->start(600);
    coin_timer->start(1000);

    // code to play sound when clicking on level
    start_level_audio->setSource(QUrl("qrc:/Sound/start_level_audio.mp3"));
    start_level_audio->setAudioOutput(start_level);
    start_level->setVolume(50);
    start_level_audio->play();

    view->showFullScreen();
}

void level2::play_music()
{
    // code to play sound when clicking on level
    start_level_audio->setSource(QUrl("qrc:/Sound/start_level_audio.mp3"));
    start_level_audio->setAudioOutput(start_level);
    start_level->setVolume(50);
    start_level_audio->play();

    // Initialize level2_music
    level2_music = new QMediaPlayer();
    level2_music->setSource(QUrl("qrc:/Sound/level2 music.mp3"));
    level2_music->setAudioOutput(base_after_base);
    base_after_base->setVolume(50);

    // Delay the level2_music audio
    QTimer::singleShot(1000, []() {
        if (level2::level2_music) {
            level2::level2_music->play();
        }
    });
}

void level2::set_level(QGraphicsScene *scene, QGraphicsView *view)
{
    scene->setSceneRect(0, 0, 1560, 870);

    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QPixmap backgroundImage(":/images/level2_background.png");
    backgroundImage = backgroundImage.scaled(scene->width(), scene->height());
    QGraphicsPixmapItem *background = new QGraphicsPixmapItem(backgroundImage);
    scene->addItem(background);
    background->setPos(0, 0);

    QPixmap level2_floor_image(":/images/level2_floor.png");
    level2_floor_image = level2_floor_image.scaled(scene->width(), 400);
    QGraphicsPixmapItem *level2_floor = new QGraphicsPixmapItem(level2_floor_image);
    level2_floor->setPos(0, scene->height() - 200);
    scene->addItem(level2_floor);
}

void level2::level_complete()
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
            if (level2::level2_music)
                level2::level2_music->stop();
        });
    });
}

level2::~level2() {}
