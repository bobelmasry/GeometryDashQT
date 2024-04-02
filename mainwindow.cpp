#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "level1.h"
#include "level2.h"
#include "itemshopwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setStyleSheet("background-image: url(://images/home-background.png);");
    ui->level1Button->setStyleSheet("color: white;" "background-color: white;");
    ui->level2Button->setStyleSheet("color: white;" "background-color: white;");
    ui->itemShopButton->setStyleSheet("color: white;" "background-color: white;");

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_level1Button_clicked()
{
    hide();
    level1 *Level1Obj = new level1();

}


void MainWindow::on_level2Button_clicked()
{
    hide();
    level2 *Level2Obj = new level2();

}


void MainWindow::on_itemShopButton_clicked()
{
    hide();
    itemShopWindow* itemWindow = new itemShopWindow(this);
    itemWindow->show();
}

