#ifndef HOMEWINDOW_H
#define HOMEWINDOW_H

#include <QDialog>

namespace Ui {
class homeWindow;
}

class homeWindow : public QDialog
{
    Q_OBJECT

public:
    explicit homeWindow(QWidget *parent = nullptr);
    ~homeWindow();

private:
    Ui::homeWindow *ui;
};

#endif // HOMEWINDOW_H
