#include "mainwindow.h"
#include "itemshopwindow.h"
#include "qstandardpaths.h"
#include "ui_mainwindow.h"
#include "level1.h"
#include "level2.h"
#include "level3.h"
#include "level4.h"
#include "level5.h"
#include <QFile>

// constructor
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // setup ui components
    ui->setupUi(this);
    // set background image for the main window
    setStyleSheet("background-image: url(://images/home-background.png);");

    // set images for level buttons
    QPixmap level1_image(":/images/level1 image.png");
    QIcon Button_Image1(level1_image);
    ui->level1Button->setIcon(Button_Image1);
    ui->level1Button->setIconSize(QSize(130, 130));

    QPixmap level2_image(":/images/level2 image.png");
    QIcon Button_Image2(level2_image);
    ui->level2Button->setIcon(Button_Image2);
    ui->level2Button->setIconSize(QSize(120, 120));

    QPixmap level3_image(":/images/level3 image.png");
    QIcon Button_Image3(level3_image);
    ui->level3Button->setIcon(Button_Image3);
    ui->level3Button->setIconSize(QSize(120, 120));

    QPixmap level4_image(":/images/level4 image.png");
    QIcon Button_Image4(level4_image);
    ui->level4Button->setIcon(Button_Image4);
    ui->level4Button->setIconSize(QSize(120, 120));

    QPixmap level5_image(":/images/level5 image.png");
    QIcon Button_Image5(level5_image);
    ui->level5Button->setIcon(Button_Image5);
    ui->level5Button->setIconSize(QSize(120, 120));

    // initialize and play main menu music
    main_menu_music->setSource(QUrl("qrc:/Sound/Main_Music.mp3"));
    main_menu_music->setAudioOutput(main_theme);
    main_theme->setVolume(50);
    main_menu_music->play();

    // get the desktop directory path
    QString desktopDir = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString skinDataReceiver = desktopDir + "/skinData.txt";

    // check if the skin data file exists on the desktop
    QFile file(skinDataReceiver);
    if (!file.exists()) {
        QString skinDataSender = ":/images/skinData.txt";

        // copy the skin data file to the desktop if it doesn't exist
        if (!copyFiley(skinDataSender, skinDataReceiver)) {
            qDebug() << "Failed to copy skin data file.";
            return;
        }
    }
}

// function to copy a file from source to destination
bool MainWindow::copyFiley(QString &sourceFile, QString &destinationFile)
{
    QFile source(sourceFile);
    if (!source.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Error opening source file";
        return false;
    }

    QFile destination(destinationFile);
    if (!destination.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Error opening destination file";
        source.close();
        return false;
    }

    QTextStream in(&source);
    QTextStream out(&destination);

    // read from source and write to destination line by line
    while (!in.atEnd()) {
        QString line = in.readLine();
        out << line << "\n";
    }

    source.close();
    destination.close();

    return true;
}

// destructor for MainWindow, cleans up the ui
MainWindow::~MainWindow()
{
    delete ui;
}

// slot function for level 1 button click event
void MainWindow::on_level1Button_clicked()
{
    hide(); // hide the main window
    level1 *Level1Obj = new level1(); // create level 1 object
    main_menu_music->stop(); // stop main menu music
}

// slot function for level 2 button click event
void MainWindow::on_level2Button_clicked()
{
    hide(); // hide the main window
    level2 *Level2Obj = new level2(); // create level 2 object
    main_menu_music->stop(); // stop main menu music
}

// slot function for level 3 button click event
void MainWindow::on_level3Button_clicked()
{
    hide(); // hide the main window
    level3 *Level3Obj = new level3(); // create level 3 object
    main_menu_music->stop(); // stop main menu music
}

// slot function for item shop button click event
void MainWindow::on_itemShopButton_clicked()
{
    hide(); // hide the main window
    itemShopWindow* itemWindow = new itemShopWindow(this); // create item shop window object
    itemWindow->show(); // show item shop window
    main_menu_music->stop(); // stop main menu music
}

// slot function for level 4 button click event
void MainWindow::on_level4Button_clicked()
{
    hide(); // hide the main window
    level4*level4obj=new level4(); // create level 4 object
    main_menu_music->stop(); // stop main menu music
}

// slot function for level 5 button click event
void MainWindow::on_level5Button_clicked()
{
    hide(); // hide the main window
    level5*level5obj=new level5(); // create level 5 object
    main_menu_music->stop(); // stop main menu music
}
