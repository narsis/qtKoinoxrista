#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QString>
#include <QSize>
#include <QPoint>
#include <QSettings>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    void setDatabase(QString dbAddr) {m_dbAddress = dbAddr;}
    void setDatabaseUser(QString dbUser) {m_dbUser = dbUser;}
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
    bool m_isMaximized;
    QSize m_dlgSize;
    QPoint m_dlgPos;
    QSettings* mp_Settings;

};

#endif // DIALOG_H
