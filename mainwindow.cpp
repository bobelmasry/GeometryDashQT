#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "level1.h"
#include "level2.h"
#include "level3.h"
#include "itemshopwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setStyleSheet("background-image: url(://images/home-background.png);");

    //level1 image
    QPixmap level1_image(":/images/level1 image.png");
    QIcon Button_Image1 (level1_image);
    ui->level1Button->setIcon(Button_Image1);
    ui->level1Button->setIconSize(QSize(130,130));


    //level2 image
    QPixmap level2_image(":/images/level2 image.png");
    QIcon Button_Image2 (level2_image);
    ui->level2Button->setIcon(Button_Image2);
    ui->level2Button->setIconSize(QSize(120,120));



    ui->itemShopButton->setStyleSheet("color: white;" "background-color: white;");

    main_menu_music->setSource(QUrl("qrc:/Sound/Main_Music.mp3"));
    main_menu_music->setAudioOutput(main_theme);
    main_theme->setVolume(50);
    main_menu_music->play();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_level1Button_clicked()
{
    hide();
    level1 *Level1Obj = new level1();
    main_menu_music->stop();



}


void MainWindow::on_level2Button_clicked()
{
    hide();
    level2 *Level2Obj = new level2();
    main_menu_music->stop();

}

void MainWindow::on_level3Button_clicked()
{
    hide();
    level3 *Level3Obj = new level3();
    main_menu_music->stop();

}


void MainWindow::on_itemShopButton_clicked()
{
    hide();
    itemShopWindow* itemWindow = new itemShopWindow(this);
    itemWindow->show();
    main_menu_music->stop();

}

