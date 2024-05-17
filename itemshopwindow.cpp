#include "itemshopwindow.h"
#include "ui_itemshopwindow.h"
#include "mainwindow.h"
#include <QFile>
#include <QStringList>
#include <QDebug>
#include <QLabel>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QStandardPaths>
#include <QDir>
#include <QCoreApplication>

itemShopWindow::itemShopWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::itemShopWindow)
{
    ui->setupUi(this);

    QString desktopDir = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString skinDataReceiver = desktopDir + "/skinData.txt";

    // Check if the file already exists
    QFile file(skinDataReceiver);
    if (!file.exists()) {
        QString skinDataSender = ":/images/skinData.txt";

        // Copy the file only if it doesn't exist on the desktop
        if (!copyFile(skinDataSender, skinDataReceiver)) {
            qDebug() << "Failed to copy skin data file.";
            return;
        }
    }

    set_upUI();

    set_up_images();
}

bool itemShopWindow::copyFile(const QString &sourceFile, const QString &destinationFile)
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

    while (!in.atEnd()) {
        QString line = in.readLine();
        out << line << "\n";
    }

    source.close();
    destination.close();

    return true;
}


void itemShopWindow::set_up_images()
{
    QPixmap skin1(":/images/crusher_skin.png");
    QPixmap skin2(":/images/destroyer_skin.png");
    QPixmap skin3(":/images/bob_skin.png");

    ui->skin1_image->setPixmap(skin1.scaled(100, 100, Qt::KeepAspectRatio));
    ui->skin2_image->setPixmap(skin2.scaled(100, 100, Qt::KeepAspectRatio));
    ui->skin3_image->setPixmap(skin3.scaled(100, 100, Qt::KeepAspectRatio));

}


void itemShopWindow::set_upUI()
{
    QString desktopDir = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString skinDataFilePath = desktopDir + "/skinData.txt";
    QString coinDataFilePath = desktopDir + "/data.txt";

    // Open skin data file
    QFile skinDataFile(skinDataFilePath);
    if (!skinDataFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open skin data file: " << skinDataFile.errorString();
        return;
    }

    // Open coin data file
    QFile readCoins(coinDataFilePath);
    if (!readCoins.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open coin data file: " << readCoins.errorString();
        return;
    }

    // Read coin data
    QString coin_num;
    QTextStream coinDataStream(&readCoins);
    while (!coinDataStream.atEnd()) {
        coin_num = coinDataStream.readLine().trimmed();
    }

    ui->coinLabel->setText("Coins: " + coin_num);

    // Read skin data
    QTextStream skinDataIn(&skinDataFile);
    // Process skin data as needed

    // Read the first line to skip the header if there's one
    QString headerLine = skinDataIn.readLine();

    // Assuming the file has multiple lines for multiple skins
    while (!skinDataIn.atEnd()) {
        QString line = skinDataIn.readLine();
        QStringList fields = line.split(',');

        QString skinName = fields.value(0);
        QString skinPrice = fields.value(1);
        QString skinIsOwned = fields.value(2);
        QString skinIsEquipped = fields.value(3);
        QString skinImagePath = fields.value(4).trimmed().replace("\"", "");

        // Assuming you have labels or other UI elements to display this information
        if (skinName == "Crusher") {
            ui->skin1_price->setText(skinPrice);

            if (skinIsOwned == "TRUE") {
                if (skinIsEquipped == "TRUE") {
                    ui->skin1_buy_equip->setText("Unequip");
                } else {
                    ui->skin1_buy_equip->setText("Equip");
                }
            } else {
                ui->skin1_buy_equip->setText("Buy");
            }
        } else if (skinName == "Destroyer") {
            ui->skin2_price->setText(skinPrice);

            if (skinIsOwned == "TRUE") {
                if (skinIsEquipped == "TRUE") {
                    ui->skin2_buy_equip->setText("Unequip");
                } else {
                    ui->skin2_buy_equip->setText("Equip");
                }
            } else {
                ui->skin2_buy_equip->setText("Buy");
            }
        } else if (skinName == "Bob") {
            ui->skin3_price->setText(skinPrice);

            if (skinIsOwned == "TRUE") {
                if (skinIsEquipped == "TRUE") {
                    ui->skin3_buy_equip->setText("Unequip");
                } else {
                    ui->skin3_buy_equip->setText("Equip");
                }
            } else {
                ui->skin3_buy_equip->setText("Buy");
            }
        }

    }

    skinDataFile.close();
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


void itemShopWindow::on_skin1_buy_equip_clicked()
{
    QString buttonText = ui->skin1_buy_equip->text();
    qDebug() << "Button text: " << buttonText;

    QString desktopDir = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString skinDataFilePath = desktopDir + "/skinData.txt";
    QString coinDataFilePath = desktopDir + "/data.txt";

    if (buttonText == "Equip" || buttonText == "Unequip") {
        // Read the contents of the file
        QFile file(skinDataFilePath);
        if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
            qDebug() << "Failed to open skin data file:" << file.errorString();
            return;
        }

        QTextStream in(&file);
        QStringList lines;
        QString line;
        while (!(line = in.readLine()).isNull()) {
            QStringList fields = line.split(',');
            if (fields[0] == "Crusher") {
                // Modify the line for Crusher to set isEquipped based on buttonText
                if (buttonText == "Equip") {
                    fields[3] = "TRUE"; // isEquipped is the fourth field
                    ui->skin1_buy_equip->setText("Unequip");
                } else if (buttonText == "Unequip" && fields[3] == "TRUE") {
                    fields[3] = "FALSE"; // isEquipped is the fourth field
                    ui->skin1_buy_equip->setText("Equip");
                }
            } else {
                // Set isEquipped to FALSE for all other skins
                fields[3] = "FALSE"; // isEquipped is the fourth field
                ui->skin2_buy_equip->setText("Equip");
                ui->skin3_buy_equip->setText("Equip");
            }
            line = fields.join(',');
            lines.append(line);
        }

        // Close the file after reading
        file.close();
        if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
            qDebug() << "Failed to open skin data file for writing:" << file.errorString();
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
        QFile coinFile(coinDataFilePath);
        if (coinFile.open(QIODevice::ReadWrite | QIODevice::Text)) {
            qDebug() << "Coin file opened successfully.";
            QTextStream coinIn(&coinFile);
            int coins = coinIn.readLine().toInt();

            QFile skinFile(skinDataFilePath);
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
                            ui->skin1_buy_equip->setText("Equip");
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

void itemShopWindow::on_skin2_buy_equip_clicked()
{
    QString buttonText = ui->skin2_buy_equip->text();
    qDebug() << "Button text: " << buttonText;

    QString desktopDir = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString skinDataFilePath = desktopDir + "/skinData.txt";
    QString coinDataFilePath = desktopDir + "/data.txt";

    if (buttonText == "Equip" || buttonText == "Unequip") {
        // Read the contents of the file
        QFile file(skinDataFilePath);
        if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
            qDebug() << "Failed to open skin data file:" << file.errorString();
            return;
        }

        QTextStream in(&file);
        QStringList lines;
        QString line;
        while (!(line = in.readLine()).isNull()) {
            QStringList fields = line.split(',');
            if (fields[0] == "Destroyer") {
                // Modify the line for Destroyer to set isEquipped based on buttonText
                if (buttonText == "Equip") {
                    fields[3] = "TRUE"; // isEquipped is the fourth field
                    ui->skin2_buy_equip->setText("Unequip");
                } else if (buttonText == "Unequip" && fields[3] == "TRUE") {
                    fields[3] = "FALSE"; // isEquipped is the fourth field
                    ui->skin2_buy_equip->setText("Equip");
                }
            } else {
                // Set isEquipped to FALSE for all other skins
                fields[3] = "FALSE"; // isEquipped is the fourth field
                ui->skin1_buy_equip->setText("Equip");
                ui->skin3_buy_equip->setText("Equip");
            }
            line = fields.join(',');
            lines.append(line);
        }

        // Close the file after reading
        file.close();
        if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
            qDebug() << "Failed to open skin data file for writing:" << file.errorString();
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
        QFile coinFile(coinDataFilePath);
        if (coinFile.open(QIODevice::ReadWrite | QIODevice::Text)) {
            qDebug() << "Coin file opened successfully.";
            QTextStream coinIn(&coinFile);
            int coins = coinIn.readLine().toInt();

            QFile skinFile(skinDataFilePath);
            if (skinFile.open(QIODevice::ReadWrite | QIODevice::Text)) {
                qDebug() << "Skin file opened successfully.";
                QTextStream skinStream(&skinFile);
                QStringList lines;
                QString line;
                while (!(line = skinStream.readLine()).isNull()) {
                    qDebug() << line;
                    if (line.contains("Destroyer")) { // Modify the line for Destroyer
                        QStringList fields = line.split(',');
                        int price = fields[1].toInt(); // Assuming price is the second field
                        if (coins >= price) {
                            coins -= price; // Deduct the price from coins
                            fields[2] = "TRUE"; // Set isOwned to TRUE
                            ui->errorLabel->setText("Destroyer Skin Bought!");
                            ui->skin2_buy_equip->setText("Equip");
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


void itemShopWindow::on_skin3_buy_equip_clicked()
{
    QString buttonText = ui->skin3_buy_equip->text();
    qDebug() << "Button text: " << buttonText;

    QString desktopDir = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString skinDataFilePath = desktopDir + "/skinData.txt";
    QString coinDataFilePath = desktopDir + "/data.txt";

    if (buttonText == "Equip" || buttonText == "Unequip") {
        // Read the contents of the file
        QFile file(skinDataFilePath);
        if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
            qDebug() << "Failed to open skin data file:" << file.errorString();
            return;
        }

        QTextStream in(&file);
        QStringList lines;
        QString line;
        while (!(line = in.readLine()).isNull()) {
            QStringList fields = line.split(',');
            if (fields[0] == "Bob") {
                // Modify the line for Bob to set isEquipped based on buttonText
                if (buttonText == "Equip") {
                    fields[3] = "TRUE"; // isEquipped is the fourth field
                    ui->skin3_buy_equip->setText("Unequip");
                } else if (buttonText == "Unequip" && fields[3] == "TRUE") {
                    fields[3] = "FALSE"; // isEquipped is the fourth field
                    ui->skin3_buy_equip->setText("Equip");
                }
            } else {
                // Set isEquipped to FALSE for all other skins
                fields[3] = "FALSE"; // isEquipped is the fourth field
                ui->skin1_buy_equip->setText("Equip");
                ui->skin2_buy_equip->setText("Equip");
            }
            line = fields.join(',');
            lines.append(line);
        }

        // Close the file after reading
        file.close();
        if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
            qDebug() << "Failed to open skin data file for writing:" << file.errorString();
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
        QFile coinFile(coinDataFilePath);
        if (coinFile.open(QIODevice::ReadWrite | QIODevice::Text)) {
            qDebug() << "Coin file opened successfully.";
            QTextStream coinIn(&coinFile);
            int coins = coinIn.readLine().toInt();

            QFile skinFile(skinDataFilePath);
            if (skinFile.open(QIODevice::ReadWrite | QIODevice::Text)) {
                qDebug() << "Skin file opened successfully.";
                QTextStream skinStream(&skinFile);
                QStringList lines;
                QString line;
                while (!(line = skinStream.readLine()).isNull()) {
                    qDebug() << line;
                    if (line.contains("Bob")) { // Modify the line for Bob
                        QStringList fields = line.split(',');
                        int price = fields[1].toInt(); // Assuming price is the second field
                        if (coins >= price) {
                            coins -= price; // Deduct the price from coins
                            fields[2] = "TRUE"; // Set isOwned to TRUE
                            ui->errorLabel->setText("Bob Skin Bought!");
                            ui->skin3_buy_equip->setText("Equip");
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
