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

    void set_upItemshop(QString &source_file, QString &destination_file);

    void on_back_clicked();


private:
    Ui::itemShopWindow *ui;
};

#endif // ITEMSHOPWINDOW_H
