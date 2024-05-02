#include "itemshopwindow.h"
#include "ui_itemshopwindow.h"
#include "mainwindow.h"
#include <QFile>
#include <QStringList>
#include <QDebug>
#include <QLabel>
#include <QTextEdit>


itemShopWindow::itemShopWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::itemShopWindow)
{
    QFile file("D://python_projects//other shit//CS//GeometryDashQT//images//skinData.csv");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << file.errorString();
        return;
    }

    QTextStream in(&file);
    QString line = in.readLine(); // read and discard the first line
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(',');

        if (fields.size() < 5) {
            qDebug() << "Invalid data format";
            continue;
        }

        // extracting fields
        QString skinName = fields[0];
        QString price = fields[1];
        QString isOwned = fields[2];
        QString isEquipped = fields[3];
        QString imagePath = fields[4].trimmed(); // to remove leading and trailing whitespace

        // displaying image
        QLabel *imageLabel = new QLabel;
        imagePath = imagePath.replace("\"", "");
        qDebug() << imagePath << '\n';
        QPixmap pixmap(imagePath);
        imageLabel->setPixmap(pixmap.scaled(100, 100));

        // displaying details
        QTextEdit *detailsTextEdit = new QTextEdit;
        detailsTextEdit->setReadOnly(true);
        QString details = QString("Skin Name: %1\nPrice: %2\nOwned: %3\nEquipped: %4").arg(skinName, price, isOwned, isEquipped);
        detailsTextEdit->setPlainText(details);
    }

    file.close();
    ui->setupUi(this);
}
itemShopWindow::~itemShopWindow()
{
    delete ui;
}

void itemShopWindow::on_backButton_clicked()
{
    hide();
    MainWindow *windowObj = new MainWindow();
    windowObj->show();

}

