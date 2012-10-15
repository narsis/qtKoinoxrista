#include <QtGui/QApplication>
#include <QDebug>
#include <QSplashScreen>
#include <QThread>
#include <QTextCodec>
#include <QSettings>
#include <QDir>
#include "dialog.h"
#include "base64.h"
#include <QtSql/QSqlDatabase>
#include <QSqlError>

class I : public QThread
{
public:
        static void sleep(unsigned long secs, QSplashScreen* splash, Dialog* dlg, QSettings* settings) {
            splash->showMessage(QObject::tr("Έλεγχος ρυθμίσεων..."), Qt::AlignLeft|Qt::AlignBottom, Qt::red);

            dlg->setSettingsFile(settings);  //Περνάμε το αρχείο ρυθμίσεων στην κλάση του παραθύρου μας
            /*Διαβάζουμε όλες τις ρυθμίσεις από το αρχείο ρυθμίσεων
              και τις μεταφέρουμε σε μεταβλητές για χρήση από την εφαρμογή*/
            QVariant windowX = settings->value("ApplicationWindow/WindowX");
            QVariant windowY = settings->value("ApplicationWindow/WindowY");
            int winX=windowX.toInt();
            int winY=windowY.toInt();

            dlg->move(winX, winY);
            QVariant winSizeX = settings->value("ApplicationWindow/WinSizeX");
            QVariant winSizeY = settings->value("ApplicationWindow/WinSizeY");

            dlg->resize(winSizeX.toInt(), winSizeY.toInt());
            QVariant maximized = settings->value("ApplicationWindow/WindowMaximized");
            if (maximized.toBool()==1)
                dlg->setWindowState(Qt::WindowMaximized);

            QVariant koinoUser = settings->value("AutomaticUserLogin/username");
            dlg->setkoinoUser(koinoUser.toString());
            QVariant koinoPass = settings->value("AutomaticUserLogin/password");

            if (!koinoPass.toString().isEmpty()) {
                std::string s = base64_decode(koinoPass.toString().toStdString());
                dlg->setKoinoPassword(s.c_str());
            }


            QThread::sleep(secs);
            splash->showMessage(QObject::tr("Σύνδεση με βάση δεδομένων..."), Qt::AlignLeft|Qt::AlignBottom, Qt::red);
            /*Διαβάζουμε τις ρυθμίσεις για τη σύνδεση με τη βάση*/
            QVariant dbAddress = settings->value("Database/database_host");
            dlg->setDatabase(dbAddress.toString());

            QVariant dbUser = settings->value("Database/database_username");
            dlg->setDatabaseUser(dbUser.toString());

            QVariant dbPass = settings->value("Database/database_user_password");
            QString tmpPass = "";
            if (!dbPass.toString().isEmpty()) {
                std::string s = base64_decode(dbPass.toString().toStdString());
                dlg->setDatabasePass(s);
                tmpPass = s.c_str();
            }

            QVariant dbName = settings->value("Database/database_to_use");
            dlg->setDatabaseName(dbName.toString());


            QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
            db.setHostName(dbAddress.toString());
            db.setDatabaseName(dbName.toString());
            db.setUserName(dbUser.toString());
            db.setPassword(tmpPass);
            if (!db.open())
                dlg->setConnected(false);
            else
                dlg->setConnected(true);
            db.close();
            QThread::sleep(secs);

        }
};

void createDefaultSettingsFile(QSettings* settingsToInitialize) {
    settingsToInitialize->beginGroup("ApplicationWindow");
    settingsToInitialize->setValue("WindowX","10");
    settingsToInitialize->setValue("WindowY", "10");
    settingsToInitialize->setValue("WinSizeX","653");
    settingsToInitialize->setValue("WinSizeY", "442");
    settingsToInitialize->setValue("WindowMaximized", "1");
    settingsToInitialize->endGroup();
    settingsToInitialize->beginGroup("Database");
    settingsToInitialize->setValue("database_host","127.0.0.1");
    settingsToInitialize->setValue("database_username", "koinoxrista");
    settingsToInitialize->setValue("database_user_password", "");
    settingsToInitialize->setValue("database_to_use","koinoxrista");
    settingsToInitialize->endGroup();
}

QSettings* initializeSettings(QApplication* app) {

    QString iniFilename = app->applicationDirPath() + QDir::separator() + "koinoxrista.ini";

    if (!QFile::exists(iniFilename)) {
        QFile iniFile(iniFilename);
        iniFile.open(QIODevice::WriteOnly);
        iniFile.close();
    }

    QSettings* settingTemp = new QSettings(iniFilename, QSettings::IniFormat);
    if (settingTemp->allKeys().isEmpty()) {
        createDefaultSettingsFile(settingTemp);
    }
    settingTemp->setFallbacksEnabled(false);
    return settingTemp;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec *utfCodec = QTextCodec::codecForName("UTF-8"); //creating new utf-8 codec
    QTextCodec::setCodecForTr(utfCodec); // setting the utf-8 codec for the tr() tags

    //Δομή για να κρατάει τις ρυθμίσεις της εφαρμογής
    QSettings* settings = initializeSettings(&a);


    //Open splash screen window
    QPixmap pixmap("splash.png");
    QSplashScreen *splash = new QSplashScreen(pixmap);
    splash->show();

    a.processEvents();

    splash->showMessage(QObject::tr(QString("Φορτώνει το πρόγραμμα...").toAscii()), Qt::AlignLeft|Qt::AlignBottom, Qt::red);
    Dialog w;
    I::sleep(2, splash, &w, settings);

    w.show();

    splash->finish(&w);
    
    return a.exec();
}
