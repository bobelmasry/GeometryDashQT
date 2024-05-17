#include "level_base.h"
#include "qdatetime.h"
#include "qgraphicsitem.h"
#include "qtimer.h"

int level_base::level=0;
int level_base::elapsedSec=0;
level_base::level_base()
{

}

void level_base::elapsed_timer_creator(QGraphicsScene *scene)
{
    QTimer*elapsed_time = new QTimer();
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
