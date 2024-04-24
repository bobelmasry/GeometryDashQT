#include "level1.h"
#include <QApplication>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include "player.h"
#include <QDateTime>


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
    level1_floor_image = level1_floor_image.scaled(scene->width(), 400);
    QGraphicsPixmapItem *level1_floor = new QGraphicsPixmapItem(level1_floor_image);
    level1_floor->setPos(0, scene->height() - 200);
    scene->addItem(level1_floor);

    Player *player = new Player(scene);
    scene->addItem(player);
    player->setPos(300, 400);
    view->setFocus();


    int initialNumOfAttempts = player->numOfAttempts;

    QDateTime startTime = QDateTime::currentDateTime();

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

    QGraphicsTextItem *elapsedTimeText = new QGraphicsTextItem();
    QFont font;
    font.setPointSize(14);
    elapsedTimeText->setFont(font);
    elapsedTimeText->setDefaultTextColor(Qt::white);
    scene->addItem(elapsedTimeText);
    int textWidth = 100;
    elapsedTimeText->setPos(scene->width() - textWidth - 200, 0);

    QTimer *timeCheck = new QTimer();
    // Timer to update stopwatch display
    QTimer *stopwatchTimer = new QTimer();
    QObject::connect(stopwatchTimer, &QTimer::timeout, [=]() {
        // Calculate elapsed time since start
        qint64 elapsedMs = startTime.msecsTo(QDateTime::currentDateTime());
        int elapsedSec = elapsedMs / 1000; // Convert to seconds

        qDebug() << "Elapsed Time:" << elapsedSec;

        elapsedTimeText->setPlainText(QString("Elapsed Time: %1 seconds").arg(elapsedSec));
        if (elapsedSec >= 30) {
            qDebug() << "Elapsed Time: " << elapsedSec;
            qDebug() << "Initial Number of Attempts: " << initialNumOfAttempts;
            qDebug() << "Current Number of Attempts: " << player->numOfAttempts;

            if (player->numOfAttempts == initialNumOfAttempts) {
                time->stop(); // Stop creating enemies
                time3->stop(); // Stop creating coins

                // Display "Game Over" message
                QGraphicsTextItem *gameOverText = new QGraphicsTextItem("Game Over");
                QFont gameOverFont;
                gameOverFont.setPointSize(30); // Set font size
                gameOverText->setFont(gameOverFont); // Apply font to the text item
                gameOverText->setDefaultTextColor(Qt::white);
                gameOverText->setPos((scene->width() - gameOverText->boundingRect().width()) / 2,
                                     (scene->height() - gameOverText->boundingRect().height()) / 2);
                scene->addItem(gameOverText);

                // Close the window after 2 seconds
                QTimer::singleShot(2000, view, &QGraphicsView::close);

                timeCheck->stop();
            }
        }
    });
    stopwatchTimer->start(1000); // Update every second
    timeCheck->start(1000);




    QPropertyAnimation *animation = new QPropertyAnimation(view, "horizontalScrollBar");

    // Set the property name
    animation->setPropertyName("value");

    // Set the start and end values
    animation->setStartValue(0);
    animation->setEndValue(scene->width() - view->width());

    // Set the duration of the animation
    animation->setDuration(10000);

    // Set the loop count to make the animation repeat infinitely
    animation->setLoopCount(-1);


    animation->start();

}

level1::~level1() {}
