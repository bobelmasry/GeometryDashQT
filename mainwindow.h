#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QAudioOutput>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void on_level1Button_clicked();

    void on_level2Button_clicked();

    void on_itemShopButton_clicked();

private:
    Ui::MainWindow *ui;
    QAudioOutput* main_theme = new QAudioOutput();
    QMediaPlayer* main_menu_music = new QMediaPlayer();



};
#endif // MAINWINDOW_H
