#ifndef ITEMSHOPWINDOW_H
#define ITEMSHOPWINDOW_H

#include <QDialog>

namespace Ui {
class itemShopWindow;
}

class itemShopWindow : public QDialog
{
    Q_OBJECT

public:
    explicit itemShopWindow(QWidget *parent = nullptr);
    ~itemShopWindow();

private slots:
    void on_backButton_clicked();

    void on_skin1Button_clicked();

    void on_skin2Button_clicked();

    void on_skin3Button_clicked();

private:
    Ui::itemShopWindow *ui;
};

#endif // ITEMSHOPWINDOW_H
