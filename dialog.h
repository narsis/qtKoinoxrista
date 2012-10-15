#ifndef DIALOG_H
#define DIALOG_H

#include <QApplication>
#include <QDialog>
#include <QChar>
#include <QString>
#include <QStringList>
#include <QSize>
#include <QPoint>
#include <QSettings>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlTableModel>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

    /*Override τη virtual μέθοδο show από την κλάση QDialog
      για να μπορέσουμε να αρχικοποιήσουμε το παράθυρο όπως θέλουμε*/
    void show();

    void setDatabase(QString dbAddr) {m_dbAddress = dbAddr;}
    void setDatabaseUser(QString dbUser) {m_dbUser = dbUser;}
    void setDatabaseName(QString dbName) {m_dbName = dbName;}
    void setDatabasePass (std::string dbPass) {m_dbPass = dbPass.c_str();}
    void setConnected (bool connected) {m_connected = connected;}
    void setUserID (QString userID) {m_userID = userID;}
    void setkoinoUser (QString koinoUser) {m_koinoUser = koinoUser;}
    void setKoinoPassword (QString koinoPass) {m_koinoPassword = koinoPass;}

    void setDialogMaximised(bool isDlgMaximized) {m_isMaximized = isDlgMaximized;}
    void setDialogSize (QSize size, QPoint pos) {m_dlgSize = size; m_dlgPos = pos;}
    void setSettingsFile(QSettings* settings) {mp_Settings = settings;}


protected:
    void closeEvent(QCloseEvent *event);
    
private:
    Ui::Dialog *ui;

    void writeSettings();
    bool userReallyWantsToQuit();

    //Μεταβλητές για διάφορες ρυθμίσεις του προγράμματος
    QString m_dbAddress;
    QString m_dbUser;
    QString m_dbPass;
    QString m_dbName;
    bool m_connected;

    QSqlTableModel* dbTableModel;

    QString m_userID;
    bool m_userIsAdmin;

    QString m_koinoUser;
    QString m_koinoPassword;

    QSqlDatabase dbHandle;

    bool m_isMaximized;


    QSize m_dlgSize;
    QPoint m_dlgPos;
    QSettings* mp_Settings;

    /*Private μέθοδοι για χρήση εντός της κλάσης*/
    void state_not_connected();
    void state_not_logged_in();
    void state_logged_in();


    /*Διάφορα slots για να καθορίσουμε συμπεριφορα*/
private slots:
    void dbConnect();
    void dbSaveSettings();
    void koinoConnect();
    void koinoSaveSettings();
    void toolboxSelChanged(int);
    void dbInitialize();
    void manageUsers();
    void cancelUserTable();
    void saveUserTable();
    void newUser();
    void deleteUser();
};

#endif // DIALOG_H
