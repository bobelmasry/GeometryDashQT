#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "level1_1.h"

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
    level1_1 *Level1Obj = new level1_1();

}

