#include "level_base.h"
#include "qdatetime.h"
#include "qgraphicsitem.h"
#include "qtimer.h"

// Initializes static members of the level_base class
int level_base::level=0;
int level_base::elapsedSec=0;

// constructor
level_base::level_base()
{

}

// function to create and start the elapsed timer
void level_base::elapsed_timer_creator(QGraphicsScene *scene)
{
    // creates a new QTimer object to update the elapsed time every second
    QTimer*elapsed_time = new QTimer();
    elapsed_time->start(1000); // sets the timer to trigger every 1 second

    // creates a new QGraphicsTextItem to display the elapsed time
    QGraphicsTextItem *elapsedTimeText = new QGraphicsTextItem();

    // sets the font size and color for the text item
    QFont font;
    font.setPointSize(14);
    elapsedTimeText->setFont(font);
    elapsedTimeText->setDefaultTextColor(Qt::white);

    // adds the text item to the sene
    scene->addItem(elapsedTimeText);

    // sets position of the text item on the scene
    int textWidth = 100;
    elapsedTimeText->setPos(scene->width() - textWidth - 1000, 0);

    // gets the current data and time as the start time
    QDateTime startTime = QDateTime::currentDateTime();

    // creates another QTimer object to update the elapsed time text
    QTimer *stopwatchTimer = new QTimer();
    QObject::connect(stopwatchTimer, &QTimer::timeout, [=]()
                     {
                         // Calculate elapsed time since start
                         qint64 elapsedMs = startTime.msecsTo(QDateTime::currentDateTime());
                         int elapsedSec = elapsedMs / 1000; // Convert to seconds

                         // updates the text item with the elapsed time in seconds
                         elapsedTimeText->setPlainText(QString("Elapsed Time: %1 seconds").arg(elapsedSec));

                     });

    // starts the stopwatch timer with a 1 second interval
    stopwatchTimer->start(1000);
}
