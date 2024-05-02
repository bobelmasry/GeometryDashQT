#include "coin.h"
#include <QGraphicsScene>
#include <QTimer>
#include <QDebug>
#include "player.h"
#include <QGraphicsPixmapItem>
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>
#include <typeinfo>

QList<coin*>coin::coins;

coin::coin() {

    coins.append(this);

    setPixmap(QPixmap("://images/coin.png").scaled(100,100));

    int randomX = QRandomGenerator::global()->bounded(600, 800 - pixmap().width());

    // makes sure that coin isn't colliding with any enemies on spawn
    QList<QGraphicsItem*> colliding_items;
    do {
        setPos(randomX+1300, 500 );
        colliding_items = collidingItems();
        if (!colliding_items.isEmpty()) {
            randomX = QRandomGenerator::global()->bounded(400, 600 - pixmap().width());
        }
    } while (!colliding_items.isEmpty()); // repeats until no collisions are detected

    QTimer *timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(move()));
    timer->start(30);
}

void coin::updateTextFile() {
    QFile file("D://python_projects//other shit//CS//GeometryDashQT//images//data.txt");
    if (file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        qDebug() << "File opened successfully.";
        QTextStream in(&file);
        int value = in.readLine().toInt();
        value++; // Increment the value

        file.seek(0);
        QTextStream out(&file);
        out << value;

        file.close();
        qDebug() << "File closed.";
    } else {
        qDebug() << "Failed to open file for reading and writing. Error:" << file.errorString();
    }

}


void coin::move() {
    setPos(x() - 15, y());
    QList<QGraphicsItem*> colliding_items = collidingItems();

    if (x() + pixmap().width() < pixmap().width()) {
        scene()->removeItem(this);
        coins.removeOne(this);
        delete this;
        return;
    }

    for (int i = 0; i < colliding_items.size(); ++i) {
        if (typeid(*(colliding_items[i])) == typeid(Player)) {
            Player *player = dynamic_cast<Player*>(colliding_items[i]);
            player->increase();
            updateTextFile();
                scene()->removeItem(this);

            coins.removeOne(this);
                delete this;
                return;
        }
    }
}
