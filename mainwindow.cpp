#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "level1.h"
#include "level2.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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

