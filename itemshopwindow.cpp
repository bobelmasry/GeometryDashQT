#include "itemshopwindow.h"
#include "ui_itemshopwindow.h"
#include "mainwindow.h"
#include <QFile>
#include <QStringList>
#include <QDebug>
#include <QLabel>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QStandardPaths>

itemShopWindow::itemShopWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::itemShopWindow)
{
    ui->setupUi(this);

    QString desktopDir = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString skinData_sender = ":/images/skinData.txt";
    QString skinData_reciver = desktopDir + "/skinData.txt"; // Verify the correctness of this path

    set_upItemshop(skinData_sender, skinData_reciver);
}

void itemShopWindow::set_upItemshop(QString &source_file, QString &destination_file)
{
    QFile sourceFile(source_file);
    if (!sourceFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Error opening source file";
        return;
    }

    QFile destinationFile(destination_file);
    if (!destinationFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Error opening destination file";
        sourceFile.close();
        return;
    }

    QTextStream inputStream(&sourceFile);
    QTextStream outputStream(&destinationFile);

    while (!inputStream.atEnd()) {
        QString line = inputStream.readLine();
        outputStream << line << "\n";
    }

    sourceFile.close();
    destinationFile.close();
}

void itemShopWindow::on_back_clicked()
{
    hide();
    MainWindow*mainwindow = new MainWindow;
    mainwindow->show();
}

itemShopWindow::~itemShopWindow()
{
    delete ui;
}
