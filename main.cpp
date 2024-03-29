#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QGraphicsRectItem>
#include "player.h"
#include <QGraphicsPixmapItem>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QGraphicsScene *scene = new QGraphicsScene();
    scene->setSceneRect(0, 0, 800, 600);

    QGraphicsView *view = new QGraphicsView(scene);
    view->setFixedSize(800, 600);
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
    player->setPos(scene->width() / 2 - player->rect().width() / 2, scene->height() - player->rect().height());

    view->setFocus();

    QTimer *time2 = new QTimer();
    QObject::connect(time2, SIGNAL(timeout()), player, SLOT(advance()));
    time2->start(50);

    QTimer *time = new QTimer();
    QObject::connect(time, SIGNAL(timeout()), player, SLOT(createEnemy()));
    QObject::connect(time, SIGNAL(timeout()), player, SLOT(createCoin()));
    time->start(2000);

    view->show();

    return a.exec();
}
