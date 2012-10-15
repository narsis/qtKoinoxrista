#include "dialog.h"
#include "ui_dialog.h"
#include <QDebug>
#include <QCloseEvent>
#include <QMessageBox>
#include <QPixmap>
#include "base64.h"
#include <QSqlQuery>
#include <QSqlResult>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QDir>
#include <QModelIndex>

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
    ui->koinoNameEdit->setText(this->m_koinoUser);
    ui->koinoPassEdit->setText(this->m_koinoPassword);

    /*Εάν δεν είναι συνδεδεμένη η βάση
      γκριζάρουμε τα πεδία που δεν μπορούν να χρησιμοποιηθούν*/
    if (!m_connected)
        state_not_connected();
    else {
        dbHandle = QSqlDatabase::addDatabase("QMYSQL");
        dbHandle.setHostName(m_dbAddress);
        dbHandle.setUserName(m_dbUser);
        dbHandle.setPassword(m_dbPass);
        dbHandle.setDatabaseName(m_dbName);

        if (!dbHandle.open()) {
            ui->labelStatus->setText(dbHandle.lastError().databaseText());
            state_not_connected();
        } else {
            state_not_logged_in();
            m_connected=true;
            ui->toolBox->setCurrentIndex(1);
        }
    }
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
    QSqlQuery query(dbHandle);
    if (!query.exec("SELECT username, password, idusers, isAdmin FROM users WHERE username='" + ui->koinoNameEdit->text() + "'")) {
        QMessageBox::warning(this, tr("Λάθος - Κοινόχρηστα"), tr("Υπήρξε λάθος στην αναζήτηση. Δοκιμάστε ξανά."));
    } else {
        if (query.size() == 1) {
           query.next();
           QString username = query.value(0).toString();
           QString password = query.value(1).toString();
           int userid = query.value(2).toInt();
           bool isAdmin = query.value(3).toBool();
           if (password == ui->koinoPassEdit->text()) {
               m_userID=userid;
               m_userIsAdmin=isAdmin;
               state_logged_in();
               ui->toolBox->setCurrentIndex(2);
           }
        } else {
            QMessageBox::warning(this, tr("Κοινόχρηστα"), tr("Ο χρήστης δεν υπάρχει!"));
        }
    }
}

void Dialog::koinoSaveSettings() {
    mp_Settings->setValue("AutomaticUserLogin/username", ui->koinoNameEdit->text());
    std::string s = ui->koinoPassEdit->text().toStdString();
    std::string encoded = base64_encode(reinterpret_cast<const unsigned char*>(s.c_str()), s.length());
    mp_Settings->setValue("AutomaticUserLogin/password", encoded.c_str());

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
            ui->koinoNameEdit->setText(this->m_koinoUser);
            ui->koinoPassEdit->setText(this->m_koinoPassword);
            break;
        case 2:
            /*Άνοιξε το μενού των μηνυμάτων*/
            break;

        case 3:
            /*Άνοιξε η καρτέλα διαχείρισης χρηστών*/
            if (!m_userIsAdmin)
                break;
            {
                QSqlTableModel* model = new QSqlTableModel(this, dbHandle);
                dbTableModel = model;
                model->setTable("users");
                model->setEditStrategy(QSqlTableModel::OnManualSubmit);
                model->select();
                model->setHeaderData(1, Qt::Horizontal, tr("Όνομα χρήστη"));
                model->setHeaderData(2, Qt::Horizontal, tr("Κωδικός"));
                model->setHeaderData(3, Qt::Horizontal, tr("Σχόλια"));
                model->setHeaderData(4, Qt::Horizontal, tr("Είναι διαχειριστής"));

                ui->tableViewUsers->setModel(model);
                ui->tableViewUsers->hideColumn(0);
                ui->tableViewUsers->show();
            }
            break;
    }
}

void Dialog::state_not_connected() {
    /*Αφού δεν είμαστε συνδεδεμένοι στη βάση απενεργοποιούμε
      επιλογές που δεν έχουν νόημα*/
    ui->toolBox->setItemEnabled(1, false);
    ui->toolBox->setItemEnabled(2, false);
    ui->toolBox->setItemEnabled(3, false);

    ui->btn_backup->setEnabled(false);
    ui->btn_manage->setEnabled(false);
}

void Dialog::state_not_logged_in() {
    /*Αφού συνδεθήκαμε με βάση, ενεργοποιούμε το 2ο γκρουπάκι
      από το toolbox για να μπορούμε να συνδεθούμε */
    ui->toolBox->setItemEnabled(1, true);
    ui->toolBox->setItemEnabled(2, false);
    ui->toolBox->setItemEnabled(3, false);

    ui->btn_backup->setEnabled(false);
    ui->btn_manage->setEnabled(false);
}

void Dialog::state_logged_in() {
    ui->toolBox->setItemEnabled(0,true);
    ui->toolBox->setItemEnabled(1, true);
    ui->toolBox->setItemEnabled(2, true);
    ui->toolBox->setItemEnabled(3, false);

    ui->btn_backup->setEnabled(m_userIsAdmin);
    ui->btn_manage->setEnabled(m_userIsAdmin);
}

void Dialog::dbInitialize() {
    if (!m_connected) {
        dbHandle = QSqlDatabase::addDatabase("QMYSQL");
        dbHandle.setHostName(ui->dbHostEdit->text());
        dbHandle.setUserName(ui->dbUsernameEdit->text());
        dbHandle.setPassword(ui->dbPasswordEdit->text());
        dbHandle.setDatabaseName("mysql");
        if (dbHandle.open()) {
            m_connected = true;
            m_dbAddress = ui->dbHostEdit->text();
            m_dbUser = ui->dbUsernameEdit->text();
            m_dbPass = ui->dbPasswordEdit->text();
        }
        else {
            QMessageBox::warning(this, tr("Κοινόχρηστα - Δεν έχει γίνει σύνδεση με τη βάση"), tr("Πριν γίνει αρχικοποίηση της βάσης πρέπει πρώτα να γίνει σύνδεση στη βάση"));
            return;
        }
    }

    QMessageBox::StandardButton ret;
    ret = QMessageBox::warning(this, tr("Κοινόχρηστα"), tr("Προσοχή !!!! Τα όποια δεδομένα τυχόν υπάρχουν στη βάση θα σβηστούν! Θέλετε να συνεχίσετε;"), QMessageBox::Yes|QMessageBox::No);
    if (ret == QMessageBox::Yes) {
        QFile* h_file = new QFile(QApplication::applicationDirPath() + QDir::separator() + "skeletondb.sql");
        h_file->open(QFile::ReadOnly);
        QString sqlScript = h_file->readAll();
        QSqlQuery query(dbHandle);
        QStringList queries = sqlScript.split(QChar(';'));

        foreach (QString queryString, queries) {

            if (!query.exec(queryString)) {
            QMessageBox::warning(this, tr("Κοινόχρηστα ΛΑΘΟΣ"), tr("Δεν μπόρεσε να γίνει αρχικοποίηση της βάσης: ")+query.lastError().databaseText());
            ui->labelStatus->setText(tr("Λάθος στη βάση : ")+query.lastError().databaseText());
            }
        }
        if (!query.exec("INSERT INTO `koinoxrista`.`users` (`username`, `password`, `comments`, `isAdmin`) VALUES ('admin', 'adminpass', 'Administrator account', 1);")) {
            ui->labelStatus->setText(tr("Λάθος στη βάση : ")+query.lastError().databaseText());
        }
        m_dbName = "koinoxrista";
        state_not_logged_in();
   }
}

void Dialog::manageUsers() {
    ui->toolBox->setItemEnabled(3, true);
    ui->toolBox->setItemEnabled(0,false);
    ui->toolBox->setItemEnabled(1,false);
    ui->toolBox->setItemEnabled(2,false);

    ui->btn_backup->setEnabled(false);
    ui->btn_manage->setEnabled(false);

    ui->toolBox->setCurrentIndex(3);
}


void Dialog::cancelUserTable() {
    ui->toolBox->setItemEnabled(3, false);
    ui->toolBox->setItemEnabled(0,true);
    ui->toolBox->setItemEnabled(1,true);
    ui->toolBox->setItemEnabled(2,true);

    ui->btn_backup->setEnabled(m_userIsAdmin);
    ui->btn_manage->setEnabled(m_userIsAdmin);

    ui->toolBox->setCurrentIndex(2);
}

void Dialog::saveUserTable() {
    dbTableModel->submitAll();
    ui->toolBox->setItemEnabled(3, false);
    ui->toolBox->setItemEnabled(0,true);
    ui->toolBox->setItemEnabled(1,true);
    ui->toolBox->setItemEnabled(2,true);

    ui->btn_backup->setEnabled(m_userIsAdmin);
    ui->btn_manage->setEnabled(m_userIsAdmin);

    ui->toolBox->setCurrentIndex(1);
}

void Dialog::newUser() {
    int numRows = dbTableModel->rowCount();
    dbTableModel->insertRows(numRows, 1);
}

void Dialog::deleteUser() {
   QModelIndex index = ui->tableViewUsers->currentIndex();
   int row = index.row();
   dbTableModel->removeRows(row, 1);
}
