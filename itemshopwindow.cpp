#include "itemshopwindow.h"
#include "ui_itemshopwindow.h"
#include "mainwindow.h"

itemShopWindow::itemShopWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::itemShopWindow)
{
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

