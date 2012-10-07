#include "dialog.h"
#include "ui_dialog.h"
#include <QDebug>
#include <QCloseEvent>
#include <QMessageBox>
#include <QPixmap>
#include "base64.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::show() {
    /*Συμπληρώνουμε τα πεδία με αυτά από το αρχείο ρυθμίσεων*/
    ui->dbUsernameEdit->setText(this->m_dbUser);
    ui->dbHostEdit->setText(this->m_dbAddress);
    ui->dbNameEdit->setText(this->m_dbName);
    ui->dbPasswordEdit->setText(this->m_dbPass);

    /*Εάν δεν είναι συνδεδεμένη η βάση
      γκριζάρουμε τα πεδία που δεν μπορούν να χρησιμοποιηθούν*/
    if (!m_connected)
        state_not_connected();
    else
        ui->toolBox->setCurrentIndex(2);

    QDialog::show();
}

void Dialog::closeEvent(QCloseEvent *event) {
    if (userReallyWantsToQuit()) {
        writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
}

bool Dialog::userReallyWantsToQuit() {
    /*Μέθοδος που βγάζει ένα παράθυρο διαλόγου για το εάν ο χρήστης
      θέλει σίγουρα να κλείσει την εφαρμογή και επιστρέφει true αν θέλει
      και false εάν δεν θέλει */

    QMessageBox::StandardButton ret;
    ret = QMessageBox::warning(this, tr("Κοινόχρηστα"), tr("Θέλετε όντως να κλείσετε το πρόγραμμα;"), QMessageBox::Yes|QMessageBox::No);
    if (ret == QMessageBox::Yes) {
        return true;
    } else {
        return false;
    }
}

void Dialog::writeSettings() {
    /*Μέθοδος που καλείται όταν κλείνει το παράθυρο και αποθηκεύει
      στο αρχείο ρυθμίσεων όσες ρυθμίσεις θέλουμε να "θυμάται" το πρόγραμμα
      από τρέξιμο σε τρέξιμο, όπως θέση και μέγεθος παραθύρου κτλ.*/
    qDebug() << "Test";
    mp_Settings->setValue("ApplicationWindow/WindowX", this->pos().x());
    mp_Settings->setValue("ApplicationWindow/WindowY", this->pos().y());
    mp_Settings->setValue("ApplicationWindow/WinSizeX", this->size().width());
    mp_Settings->setValue("ApplicationWindow/WinSizeY", this->size().height());
    mp_Settings->setValue("ApplicationWindow/WindowMaximized", this->isMaximized()?"1":"0");
}

void Dialog::dbConnect() {

}

void Dialog::dbSaveSettings() {
    mp_Settings->setValue("Database/database_host", ui->dbHostEdit->text());
    mp_Settings->setValue("Database/database_username", ui->dbUsernameEdit->text());
    mp_Settings->setValue("Database/database_to_use", ui->dbNameEdit->text());

    //Κάνουμε base64 κωδικοποίηση τον κωδικό για να μη φαίνεται χύμα στο αρχείο
    std::string s = ui->dbPasswordEdit->text().toStdString();
    std::string encoded = base64_encode(reinterpret_cast<const unsigned char*>(s.c_str()),s.length());
    mp_Settings->setValue("Database/database_user_password", encoded.c_str());

}

void Dialog::koinoConnect() {

}

void Dialog::koinoSaveSettings() {

}

void Dialog::toolboxSelChanged(int selection) {
    switch (selection) {
        case 0:
            /*Άνοιξε το μενού της βάσης*/
            ui->dbUsernameEdit->setText(this->m_dbUser);
            ui->dbHostEdit->setText(this->m_dbAddress);
            ui->dbNameEdit->setText(this->m_dbName);
            ui->dbPasswordEdit->setText(this->m_dbPass);
            break;
        case 1:
            /*Άνοιξε το μενού σύνδεσης στο πρόγραμμα*/
            break;
        case 2:
            /*Άνοιξε το μενού των μηνυμάτων*/
            break;


    }
}

void Dialog::state_not_connected() {
    /*Αφού δεν είμαστε συνδεδεμένοι στη βάση απενεργοποιούμε
      επιλογές που δεν έχουν νόημα*/
    ui->toolBox->setItemEnabled(1, false);
    ui->toolBox->setItemEnabled(2, false);

    ui->btn_backup->setEnabled(false);
    ui->btn_manage->setEnabled(false);
}
