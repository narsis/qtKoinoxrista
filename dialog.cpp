#include "dialog.h"
#include "ui_dialog.h"
#include <QDebug>
#include <QCloseEvent>
#include <QMessageBox>

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

void Dialog::closeEvent(QCloseEvent *event) {
    if (userReallyWantsToQuit()) {
        writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
}

bool Dialog::userReallyWantsToQuit() {
    QMessageBox::StandardButton ret;
    ret = QMessageBox::warning(this, tr("Κοινόχρηστα"), tr("Θέλετε όντως να κλείσετε το πρόγραμμα;"), QMessageBox::Yes|QMessageBox::No);
    if (ret == QMessageBox::Yes) {
        return true;
    } else {
        return false;
    }
}

void Dialog::writeSettings() {
    qDebug() << size() << " ds    " << pos();
}
