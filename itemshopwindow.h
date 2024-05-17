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
    bool copyFile(const QString &sourceFile, const QString &destinationFile);
    void set_upUI();
    void set_up_images();

private slots:
    void on_backButton_clicked();

    void on_skin1_buy_equip_clicked();

    void on_skin2_buy_equip_clicked();

    void on_skin3_buy_equip_clicked();

private:
    Ui::itemShopWindow *ui;
};

#endif // ITEMSHOPWINDOW_H
