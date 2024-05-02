#include "itemshopwindow.h"
#include "ui_itemshopwindow.h"
#include "mainwindow.h"
#include <QFile>
#include <QStringList>
#include <QDebug>

itemShopWindow::itemShopWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::itemShopWindow)
{
    QFile file("D://python_projects//other shit//CS//GeometryDashQT//images//skinData.csv");
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << file.errorString();
    }
    std::vector<std::vector<QString>> data;

    while (!file.atEnd()) {
        QString line = file.readLine();
        QStringList fields = line.split(',');

        std::vector<QString> row;
        for (const QString& field : fields) {
            row.push_back(field);
        }

        data.push_back(row);
    }


    qDebug() << data;

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

