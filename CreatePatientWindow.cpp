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
    if (ui->lePatientId->text().isEmpty()) {
        this->showFormErrormMessage("ID");
        return;
    }

    if (ui->lePatientFirstName->text().isEmpty()) {
        this->showFormErrormMessage("First Name");
        return;
    }

    if (ui->lePatientLastName->text().isEmpty()) {
        this->showFormErrormMessage("Last Name");
        return;
    }

    if (!ui->rbPatientIsFemale->isChecked() && !ui->rbPatientIsMale->isChecked()) {
        this->showFormErrormMessage("Gender");
        return;
    }

    if (ui->sbPatientHeight->text().isEmpty()) {
        this->showFormErrormMessage("Height");
        return;
    }

    if (ui->sbPatientWeight->text().isEmpty()) {
        this->showFormErrormMessage("Weight");
        return;
    }

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

void CreatePatientWindow::showFormErrormMessage(QString field)
{
    QMessageBox::critical(this, "Error", "Please complete the field <b>"+ field + "</b>.");
}


CreatePatientWindow::~CreatePatientWindow()
{
    delete ui;
}
