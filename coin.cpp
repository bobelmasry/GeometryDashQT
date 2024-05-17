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
#include <QStandardPaths>

QList<coin*>coin::coins; // stores coin objects

coin::coin() {

    coins.append(this); // adds the current coin object to the list of coins

    setPixmap(QPixmap("://images/coin.png").scaled(100,100)); // sets the pixmap for the coin

    // generates a random position for the coin within a certain range
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

    // sets up a timer to move the coin
    QTimer *timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(move()));
    timer->start(30);
}

// this function updates the text file
void coin::updateTextFile() {
    // gets the path to the desktop directory
    QString desktopDir = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString filePath = desktopDir + "/data.txt";

    // opens the file for reading and writing
    QFile file(filePath);
    if (file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        qDebug() << "File opened successfully.";
        QTextStream in(&file);
        int value = in.readLine().toInt(); // reads the current value from the file
        value++; // Increment the value

        // writes the updated value back to the file
        file.seek(0);
        QTextStream out(&file);
        out << value;

        file.close(); // closes the file
        qDebug() << "File closed.";
    } else {
        qDebug() << "Failed to open file for reading and writing. Error:" << file.errorString();
    }
}


void coin::move() {
    setPos(x() - 15, y()); // moves coin horizontally

    //checks for collisions with other items
    QList<QGraphicsItem*> colliding_items = collidingItems();

    // if the coin moves out of the scene, remove it from the scene and delete it
    if (x() + pixmap().width() < pixmap().width()) {
        scene()->removeItem(this);
        coins.removeOne(this);
        delete this;
        return;
    }

    // check if the coin collides with the player
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
