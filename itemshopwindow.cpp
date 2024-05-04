#include "itemshopwindow.h"
#include "ui_itemshopwindow.h"
#include "mainwindow.h"
#include <QFile>
#include <QStringList>
#include <QDebug>
#include <QLabel>
#include <QTextEdit>
#include <QVBoxLayout>


itemShopWindow::itemShopWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::itemShopWindow)
{
    ui->setupUi(this);
    QFile file("D://python_projects//other shit//CS//GeometryDashQT//images//skinData.csv");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << file.errorString();
        return;
    }
    QTextStream in(&file);
    QString line = in.readLine(); // read and discard the first line as name of field

    line = in.readLine();
    QStringList fields = line.split(',');

    // Extracting fields for skin 1
    QString skin1price = fields[1];
    QString skin1isOwned = fields[2];
    QString skin1isEquipped = fields[3];
    QString skin1imagePath = fields[4].trimmed(); // to remove leading and trailing whitespace
    skin1imagePath = skin1imagePath.replace("\"", "");

    qDebug() << skin1price << "\n";
    qDebug() << skin1isOwned << "\n";
    qDebug() << skin1isEquipped << "\n";
    qDebug() << skin1imagePath << "\n";


    line = in.readLine();
    fields = line.split(',');

    // Extracting fields for skin 2
    QString skin2price = fields[1];
    QString skin2isOwned = fields[2];
    QString skin2isEquipped = fields[3];
    QString skin2imagePath = fields[4].trimmed(); // to remove leading and trailing whitespace
    skin2imagePath = skin2imagePath.replace("\"", "");


    line = in.readLine();
    fields = line.split(',');

    // Extracting fields for skin 3
    QString skin3price = fields[1];
    QString skin3isOwned = fields[2];
    QString skin3isEquipped = fields[3];
    QString skin3imagePath = fields[4].trimmed(); // to remove leading and trailing whitespace
    skin3imagePath = skin3imagePath.replace("\"", "");
    QPixmap skin3Pixmap = QPixmap(skin3imagePath).scaled(100, 100);
    ui->skin3Label->setPixmap(skin3Pixmap);

    file.close();

    if (skin1isOwned == "FALSE"){
        ui->skin1Details->setText(skin1price);
        ui->skin1Button->setText("Buy");
    }
    else {
        if (skin1isOwned == "TRUE") {
            ui->skin1Button->setText("Equip");
        }
        else {
            ui->skin1Button->setText("Unequip");
        }
    }

    if (skin2isOwned == "FALSE"){
        ui->skin2Details->setText(skin2price);
        ui->skin2Button->setText("Buy");
    }
    else {
        if (skin2isOwned == "TRUE") {
            ui->skin2Button->setText("Equip");
        }
        else {
            ui->skin2Button->setText("Unequip");
        }
    }


    if (skin3isOwned == "FALSE"){
        ui->skin3Details->setText(skin3price);
        ui->skin3Button->setText("Buy");
    }
    else {
        if (skin3isOwned == "TRUE") {
            ui->skin3Button->setText("Equip");
        }
        else {
            ui->skin3Button->setText("Unequip");
        }
    }

    QFile coinFile("D://python_projects//other shit//CS//GeometryDashQT//images//data.txt");
    if (coinFile.open(QIODevice::ReadWrite | QIODevice::Text)) {
        qDebug() << "Coin file opened successfully.";
        QTextStream coinIn(&coinFile);
        int coins = coinIn.readLine().toInt();
        ui->coinLabel->setText(QString::number(coins));
    }

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


void itemShopWindow::on_skin1Button_clicked()
{
    QString buttonText = ui->skin1Button->text();
    qDebug() << "Button text: " << buttonText;

    if (buttonText == "Equip" || buttonText == "Unequip"){
        // Read the contents of the file
        QFile file("D://python_projects//other shit//CS//GeometryDashQT//images//skinData.csv");
        if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
            qDebug() << file.errorString();
            return;
        }

        QTextStream in(&file);
        QStringList lines;
        QString line;
        while (!(line = in.readLine()).isNull()) {
            if (line.contains("Crusher")) {
                // Modify the line for Crusher to set isEquipped based on buttonText
                QStringList fields = line.split(',');
                if (buttonText == "Equip") {
                    fields[3] = "TRUE"; // isEquipped is the fourth field
                    ui->skin1Button->setText("Unequip");
                } else if (buttonText == "Unequip" && fields[3] == "TRUE") {
                    fields[3] = "FALSE"; // isEquipped is the fourth field
                    ui->skin1Button->setText("Equip");
                }
                line = fields.join(',');
            }
            lines.append(line);
        }

        // Close the file after reading
        file.close();
        if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
            qDebug() << file.errorString();
            return;
        }

        QTextStream out(&file);
        for (const QString& line : qAsConst(lines)) {
            out << line << '\n';
        }

        // Close the file after writing
        file.close();
    } else {
        // Case where Button Text is "Buy"
        QFile coinFile("D://python_projects//other shit//CS//GeometryDashQT//images//data.txt");
        if (coinFile.open(QIODevice::ReadWrite | QIODevice::Text)) {
            qDebug() << "Coin file opened successfully.";
            QTextStream coinIn(&coinFile);
            int coins = coinIn.readLine().toInt();

            QFile skinFile("D://python_projects//other shit//CS//GeometryDashQT//images//skinData.csv");
            if (skinFile.open(QIODevice::ReadWrite | QIODevice::Text)) {
                qDebug() << "Skin file opened successfully.";
                QTextStream skinStream(&skinFile);
                QStringList lines;
                QString line;
                while (!(line = skinStream.readLine()).isNull()) {
                    qDebug() << line;
                    if (line.contains("Crusher")) { // Modify the line for Crusher
                        QStringList fields = line.split(',');
                        int price = fields[1].toInt(); // Assuming price is the second field
                        if (coins >= price) {
                            coins -= price; // Deduct the price from coins
                            fields[2] = "TRUE"; // Set isOwned to TRUE
                            ui->errorLabel->setText("Crusher Skin Bought!");
                            ui->skin1Button->setText("Equip");
                        } else {
                            ui->errorLabel->setText("Not Enough Coins!");
                            skinFile.close();
                            coinFile.close();
                            return;
                        }
                        line = fields.join(',');
                    }
                    lines.append(line);
                }
                // Write the modified lines back to the CSV file
                skinFile.seek(0);
                skinFile.resize(0); // Clear the file
                QTextStream skinOut(&skinFile);
                for (const QString& line : qAsConst(lines)) {
                    skinOut << line << '\n';
                }
                skinFile.close();
            } else {
                qDebug() << "Failed to open skin file for reading and writing. Error:" << skinFile.errorString();
            }

            // Write the updated coins back to the coin file
            coinFile.seek(0);
            coinFile.resize(0);
            QTextStream coinOut(&coinFile);
            coinOut << coins;
            coinFile.close();
        } else {
            qDebug() << "Failed to open coin file for reading and writing. Error:" << coinFile.errorString();
        }
    }


}



