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

QMediaPlayer* level1::level1_music = nullptr;

int level1::elapsedSec=0;

QGraphicsScene* level1::scene;
QGraphicsView* level1::view;
QTimer* level1::enemy_timer;
QTimer* level1::coin_timer;
QTimer* level1::elapsed_time;

level1::level1()
{

    scene = new QGraphicsScene();
    view = new QGraphicsView(scene);

    set_level(scene,view);

    play_music();

    Player *player = new Player(scene);
    scene->addItem(player);
    player->setPos(300,400);
    view->setFocus();

    view->showFullScreen();

    enemy_timer = new QTimer();
    QObject::connect(enemy_timer, SIGNAL(timeout()), player, SLOT(createEnemy()));
    enemy_timer->start(1000);

    coin_timer = new QTimer();
    QObject::connect(coin_timer, SIGNAL(timeout()), player, SLOT(createCoin()));
    coin_timer->start(1500);

    QTimer *player_timer = new QTimer();
    QObject::connect(player_timer, SIGNAL(timeout()), player, SLOT(advance()));
    player_timer->start(50);

    elapsed_timer_creator(scene);



}

void level1::set_level(QGraphicsScene *scene, QGraphicsView *view)
{

    scene->setSceneRect(0, 0, 1560, 870);

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
}


void level1::update_timer(QGraphicsTextItem *elapsedTimeText, int &elapsedSec)
{
    elapsedSec++;

    //qDebug() << "Elapsed Time:" << elapsedSec;

    elapsedTimeText->setPlainText(QString("Elapsed Time: %1 seconds").arg(elapsedSec));
}

void level1::elapsed_timer_creator(QGraphicsScene *scene)
{
    elapsed_time = new QTimer();
    elapsed_time->start(1000);

    QGraphicsTextItem *elapsedTimeText = new QGraphicsTextItem();
    QFont font;
    font.setPointSize(14);
    elapsedTimeText->setFont(font);
    elapsedTimeText->setDefaultTextColor(Qt::white);
    scene->addItem(elapsedTimeText);
    int textWidth = 100;
    elapsedTimeText->setPos(scene->width() - textWidth - 1000, 0);

    QDateTime startTime = QDateTime::currentDateTime();

    QTimer *stopwatchTimer = new QTimer();
    QObject::connect(stopwatchTimer, &QTimer::timeout, [=]()
                     {
                         // Calculate elapsed time since start
                         qint64 elapsedMs = startTime.msecsTo(QDateTime::currentDateTime());
                         int elapsedSec = elapsedMs / 1000; // Convert to seconds

                         //qDebug() << "Elapsed Time:" << elapsedSec;

                         elapsedTimeText->setPlainText(QString("Elapsed Time: %1 seconds").arg(elapsedSec));

                     });

    stopwatchTimer->start(1000);
}




void level1::level_complete()
{
    enemy_timer->stop();
    coin_timer->stop();

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
                elapsed_time->stop();
                QTimer::singleShot(3000, [&]() {
                    MainWindow *windowObj = new MainWindow();
                    windowObj->show();
                    level1_music->stop();
                });
}
                       );

}

void level1::play_music()
{
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

}


QMediaPlayer* level1::getLevel1Music()
{
    return level1_music;
}



level1::~level1() {}
