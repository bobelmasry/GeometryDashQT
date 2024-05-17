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

// constructor
itemShopWindow::itemShopWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::itemShopWindow)
{
    ui->setupUi(this);

    // gets the path for the skin data file
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

    // sets up the user interface
    set_upUI();

    // sets up images for skins
    set_up_images();
}

// this function is for copying a file
bool itemShopWindow::copyFile(const QString &sourceFile, const QString &destinationFile)
{
    // opens the source file for reading
    QFile source(sourceFile);
    if (!source.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Error opening source file";
        return false;
    }

    // opens the destination file for writing
    QFile destination(destinationFile);
    if (!destination.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Error opening destination file";
        source.close();
        return false;
    }

    // copies contents from source to destination
    QTextStream in(&source);
    QTextStream out(&destination);

    while (!in.atEnd()) {
        QString line = in.readLine();
        out << line << "\n";
    }

    // closes both files after copying
    source.close();
    destination.close();

    return true;
}

// this function sets up images for skins
void itemShopWindow::set_up_images()
{
    // loads skin images and sets them to corresponding ui elements
    QPixmap skin1(":/images/crusher_skin.png");
    QPixmap skin2(":/images/destroyer_skin.png");
    QPixmap skin3(":/images/bob_skin.png");

    ui->skin1_image->setPixmap(skin1.scaled(100, 100, Qt::KeepAspectRatio));
    ui->skin2_image->setPixmap(skin2.scaled(100, 100, Qt::KeepAspectRatio));
    ui->skin3_image->setPixmap(skin3.scaled(100, 100, Qt::KeepAspectRatio));

}

// this function sets up the ui
void itemShopWindow::set_upUI()
{
    QString desktopDir = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString skinDataFilePath = desktopDir + "/skinData.txt";
    QString coinDataFilePath = desktopDir + "/data.txt";

    // opens skin data file
    QFile skinDataFile(skinDataFilePath);
    if (!skinDataFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open skin data file: " << skinDataFile.errorString();
        return;
    }

    // opens coin data file
    QFile readCoins(coinDataFilePath);
    if (!readCoins.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open coin data file: " << readCoins.errorString();
        return;
    }

    // reads coin data
    QString coin_num;
    QTextStream coinDataStream(&readCoins);
    while (!coinDataStream.atEnd()) {
        coin_num = coinDataStream.readLine().trimmed();
    }

    ui->coinLabel->setText("Coins: " + coin_num);

    // reads skin data
    QTextStream skinDataIn(&skinDataFile);


    QString headerLine = skinDataIn.readLine();


    while (!skinDataIn.atEnd()) {
        QString line = skinDataIn.readLine();
        QStringList fields = line.split(',');

        QString skinName = fields.value(0);
        QString skinPrice = fields.value(1);
        QString skinIsOwned = fields.value(2);
        QString skinIsEquipped = fields.value(3);
        QString skinImagePath = fields.value(4).trimmed().replace("\"", "");


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

// destructor
itemShopWindow::~itemShopWindow()
{
    delete ui;
}

//this function handles what happens when the back button is clicked
void itemShopWindow::on_backButton_clicked()
{
    hide(); // hides the current window
    MainWindow *windowObj = new MainWindow();
    windowObj->show(); // shows the main window

}

// this function handles what happens when skin1 buy equip button is clicked
void itemShopWindow::on_skin1_buy_equip_clicked()
{
    QString buttonText = ui->skin1_buy_equip->text();
    qDebug() << "Button text: " << buttonText;

    QString desktopDir = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString skinDataFilePath = desktopDir + "/skinData.txt";
    QString coinDataFilePath = desktopDir + "/data.txt";

    if (buttonText == "Equip" || buttonText == "Unequip") {
        // reads the contents of the file
        QFile file(skinDataFilePath);
        if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
            qDebug() << "Failed to open skin data file:" << file.errorString();
            return;
        }

        QTextStream in(&file);
        QStringList lines;
        QString line;
        bool skinFound = false;
        while (!(line = in.readLine()).isNull()) {
            QStringList fields = line.split(',');
            if (fields[0] == "Crusher" && fields[2] == "TRUE") {
                skinFound = true;
                // modifies the line for Crusher to set isEquipped based on buttonText
                if (buttonText == "Equip") {
                    fields[3] = "TRUE";
                    ui->skin1_buy_equip->setText("Unequip");
                } else if (buttonText == "Unequip" && fields[3] == "TRUE") {
                    fields[3] = "FALSE";
                    ui->skin1_buy_equip->setText("Equip");
                }
            }
            line = fields.join(',');
            lines.append(line);
        }

        // Close the file after reading
        file.close();
        if (!skinFound) {
            qDebug() << "Skin not found in the data file or not owned.";
            return;
        }

        if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
            qDebug() << "Failed to open skin data file for writing:" << file.errorString();
            return;
        }

        QTextStream out(&file);
        for (const QString& line : qAsConst(lines)) {
            out << line << '\n';
        }

        // closes the file after writing
        file.close();
    } else {
        // case where Button Text is "Buy"
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
                bool skinFound = false;
                while (!(line = skinStream.readLine()).isNull()) {
                    qDebug() << line;
                    if (line.contains("Crusher")) { // modifies the line for Crusher
                        skinFound = true;
                        QStringList fields = line.split(',');
                        int price = fields[1].toInt();
                        if (coins >= price) {
                            coins -= price; // deducts the price from coins
                            fields[2] = "TRUE"; // set isOwned to TRUE
                            fields[3] = "TRUE"; // set isEquipped to TRUE after buying
                            ui->errorLabel->setText("Crusher Skin Bought!");
                            ui->skin1_buy_equip->setText("Unequip");
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
                // closes the skin file after reading
                skinFile.close();

                if (!skinFound) {
                    qDebug() << "Skin not found in the data file.";
                    return;
                }

                // writes the modified lines back to the file
                if (!skinFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
                    qDebug() << "Failed to open skin data file for writing:" << skinFile.errorString();
                    return;
                }
                QTextStream skinOut(&skinFile);
                for (const QString& line : qAsConst(lines)) {
                    skinOut << line << '\n';
                }
                skinFile.close();
            } else {
                qDebug() << "Failed to open skin file for reading and writing. Error:" << skinFile.errorString();
            }

            // writes the updated coins back to the coin file
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

// this function handles what happens when skin2 buy equip button is clicked
void itemShopWindow::on_skin2_buy_equip_clicked()
{
    QString buttonText = ui->skin2_buy_equip->text();
    qDebug() << "Button text: " << buttonText;

    QString desktopDir = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString skinDataFilePath = desktopDir + "/skinData.txt";
    QString coinDataFilePath = desktopDir + "/data.txt";

    if (buttonText == "Equip" || buttonText == "Unequip") {
        // readsthe contents of the file
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
                // modifies the line for Destroyer to set isEquipped based on buttonText
                if (buttonText == "Equip") {
                    fields[3] = "TRUE";
                    ui->skin2_buy_equip->setText("Unequip");
                } else if (buttonText == "Unequip" && fields[3] == "TRUE") {
                    fields[3] = "FALSE";
                    ui->skin2_buy_equip->setText("Equip");
                }
            }
            line = fields.join(',');
            lines.append(line);
        }

        // closes the file after reading
        file.close();
        if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
            qDebug() << "Failed to open skin data file for writing:" << file.errorString();
            return;
        }

        QTextStream out(&file);
        for (const QString& line : qAsConst(lines)) {
            out << line << '\n';
        }

        // closes the file after writing
        file.close();

    } else {
        // case where Button Text is "Buy"
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
                bool skinFound = false;
                while (!(line = skinStream.readLine()).isNull()) {
                    qDebug() << line;
                    if (line.contains("Destroyer") && line.contains("TRUE")) { // Modifies the line for Destroyer
                        skinFound = true;
                        QStringList fields = line.split(',');
                        int price = fields[1].toInt();
                        if (coins >= price) {
                            coins -= price; // deducts the price from coins
                            fields[2] = "TRUE"; // sets isOwned to TRUE
                            fields[3] = "TRUE"; // sets isEquipped to TRUE after buying
                            ui->errorLabel->setText("Destroyer Skin Bought!");
                            ui->skin2_buy_equip->setText("Unequip");
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
                // closes the skin file after reading
                skinFile.close();

                if (!skinFound) {
                    qDebug() << "Skin not found in the data file or not owned.";
                    return;
                }

                // writes the modified lines back to the file
                if (!skinFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
                    qDebug() << "Failed to open skin data file for writing:" << skinFile.errorString();
                    return;
                }
                QTextStream skinOut(&skinFile);
                for (const QString& line : qAsConst(lines)) {
                    skinOut << line << '\n';
                }
                skinFile.close();
            } else {
                qDebug() << "Failed to open skin file for reading and writing. Error:" << skinFile.errorString();
            }

            // writes the updated coins back to the coin file
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

// this function handles what happens when skin3 buy equip button is clicked
void itemShopWindow::on_skin3_buy_equip_clicked()
{
    QString buttonText = ui->skin3_buy_equip->text();
    qDebug() << "Button text: " << buttonText;

    QString desktopDir = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString skinDataFilePath = desktopDir + "/skinData.txt";
    QString coinDataFilePath = desktopDir + "/data.txt";

    if (buttonText == "Equip" || buttonText == "Unequip") {
        // reads the contents of the file
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
                // modifies the line for Bob to set isEquipped based on buttonText
                if (buttonText == "Equip") {
                    fields[3] = "TRUE";
                    ui->skin3_buy_equip->setText("Unequip");
                } else if (buttonText == "Unequip" && fields[3] == "TRUE") {
                    fields[3] = "FALSE";
                    ui->skin3_buy_equip->setText("Equip");
                }
            }
            line = fields.join(',');
            lines.append(line);
        }

        // closes the file after reading
        file.close();
        if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
            qDebug() << "Failed to open skin data file for writing:" << file.errorString();
            return;
        }

        QTextStream out(&file);
        for (const QString& line : qAsConst(lines)) {
            out << line << '\n';
        }

        // closes the file after writing
        file.close();

    } else {
        // case where Button Text is "Buy"
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
                bool skinFound = false;
                while (!(line = skinStream.readLine()).isNull()) {
                    qDebug() << line;
                    if (line.contains("Bob")) { // Modifies the line for Bob
                        skinFound = true;
                        QStringList fields = line.split(',');
                        int price = fields[1].toInt();
                        if (coins >= price) {
                            coins -= price; // deducts the price from coins
                            fields[2] = "TRUE"; // Set isOwned to TRUE
                            fields[3] = "TRUE"; // Set isEquipped to TRUE after buying
                            ui->errorLabel->setText("Bob Skin Bought!");
                            ui->skin3_buy_equip->setText("Unequip");
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
                // closes the skin file after reading
                skinFile.close();

                if (!skinFound) {
                    qDebug() << "Skin not found in the data file.";
                    return;
                }

                // Write the modified lines back to the file
                if (!skinFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
                    qDebug() << "Failed to open skin data file for writing:" << skinFile.errorString();
                    return;
                }
                QTextStream skinOut(&skinFile);
                for (const QString& line : qAsConst(lines)) {
                    skinOut << line << '\n';
                }
                skinFile.close();
            } else {
                qDebug() << "Failed to open skin file for reading and writing. Error:" << skinFile.errorString();
            }

            // writes the updated coins back to the coin file
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
