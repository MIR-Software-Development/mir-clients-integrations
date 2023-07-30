#include "CreatePatientWindow.h"
#include "ui_CreatePatientWindow.h"

#include <QMessageBox>

CreatePatientWindow::CreatePatientWindow(QWidget *parent, QString protocol):
    QDialog(parent),
    ui(new Ui::CreatePatientWindow)
{
    ui->setupUi(this);

    this->currentProtocol = protocol;

    QStringList ethnicGroups;

    if (protocol == "HL7") {
        ethnicGroups << "2054 B" << "2028 A" << "2106 W";
    }
    else if (protocol == "WXP") {
        ethnicGroups << "Caucasian" << "Oriental" << "Chinese" << "Japanese" << "Polynesian" << "North Indian" << "South Indian" << "Pakistan" << "African descent" << "Not defined";
    } else {
        QMessageBox::information(this, "Error", "This protocol does not exist.");
        this->close();
    }

    ui->cbEthnicGroups->insertItems(0, ethnicGroups);

    connect(ui->pbCreatePatient, SIGNAL(clicked()), this, SLOT(createPatient()));
}

void CreatePatientWindow::createPatient()
{
    Patient *patient = new Patient(
        ui->lePatientId->text(),
        ui->lePatientFirstName->text(),
        ui->lePatientLastName->text(),
        ui->dePatientBirthDate->text(),
        ui->rbPatientIsFemale->isChecked() ? 1 : 0,
        ui->sbPatientHeight->value(),
        ui->sbPatientWeight->value(),
        ui->cbEthnicGroups->currentIndex(),
        ui->tePatientNotes->toPlainText()
    );

    QString message = NULL;

    if (this->currentProtocol == "HL7") {
        message = HL7::createPatientMessage(patient);
    } else {
        message = WXP::createPatientMessage(patient);
    }

    emit patientCreated(message);

    this->close();
}


CreatePatientWindow::~CreatePatientWindow()
{
    delete ui;
}
