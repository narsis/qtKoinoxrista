#include <QtGui/QApplication>
#include <QDebug>
#include <QSplashScreen>
#include <QThread>
#include <QTextCodec>
#include <QSettings>
#include <QDir>
#include "dialog.h"

class I : public QThread
{
public:
        static void sleep(unsigned long secs, QSplashScreen* splash, QDialog* dlg, QSettings* settings) {
            splash->showMessage(QObject::tr("Έλεγχος ρυθμίσεων..."), Qt::AlignLeft|Qt::AlignBottom, Qt::red);

            settings->allKeys().isEmpty()

            //Εάν δεν υπάρχει το αρχείο ή αν υπάρχει κάποιο λάθος ξαναφτιάχνουμε το αρχείο ρυθμίσεων
            if (settings->status() != QSettings::NoError) {
                splash->showMessage(QObject::tr("Λάθος στις ρυθμίσεις... Αρχικοποίηση αρχείου ρυθμίσεων"), Qt::AlignLeft|Qt::AlignBottom, Qt::red);
                settings->beginGroup("General");
                settings->setValue("maximized", "0");
                settings->setValue("window_size", "test");
                settings->setValue("window_position", "0,0");
                settings->endGroup();
                settings->beginGroup("Database");
                settings->setValue("dbAddress", "127.0.0.1");
                settings->setValue("dbUser", "root");
                settings->setValue("dbName", "koinoxrista");
                settings->endGroup();
            }

            QThread::sleep(secs);
            splash->showMessage(QObject::tr("Σύνδεση με βάση δεδομένων..."), Qt::AlignLeft|Qt::AlignBottom, Qt::red);
            QThread::sleep(secs);

        }
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec *utfCodec = QTextCodec::codecForName("UTF-8"); //creating new utf-8 codec
    QTextCodec::setCodecForTr(utfCodec); // setting the utf-8 codec for the tr() tags

    //Δομή για να κρατάει τις ρυθμίσεις της εφαρμογής
    QSettings* settings = new QSettings(a.applicationDirPath() + QDir::separator() +"koinoxrista.ini", QSettings::IniFormat);
    qDebug() << settings->fileName();
    settings->setFallbacksEnabled(false);

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
