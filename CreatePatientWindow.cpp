#include "CreatePatientWindow.h"
#include "ui_CreatePatientWindow.h"

#include <QMessageBox>

CreatePatientWindow::CreatePatientWindow(QWidget *parent, QString protocol):
    QDialog(parent),
    ui(new Ui::CreatePatientWindow)
{
    ui->setupUi(this);

    if (protocol == "HL7") {
        // Combobox to change.
    }
    else if (protocol == "WXP") {
        // Combobox to change.
    } else {
        QMessageBox::information(this, "Error", "This protocol does not exist.");
    }
}

CreatePatientWindow::~CreatePatientWindow()
{
    delete ui;
}
