#include "CreatePatientSessionWindow.h"
#include "ui_CreatePatientSessionWindow.h"

#include <QMessageBox>

CreatePatientSessionWindow::CreatePatientSessionWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreatePatientSessionWindow)
{
    ui->setupUi(this);

    // TODO: Implement protocol switching.

    connect(ui->pbCreateSession, SIGNAL(clicked()), this, SLOT(createSession()));
}

void CreatePatientSessionWindow::createSession()
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

    Patient *patient = new Patient(
        ui->lePatientId->text(),
        ui->lePatientFirstName->text(),
        ui->lePatientLastName->text(),
        ui->dePatientBirthDate->text()
    );

    QString message = NULL;

    // TODO: Implement protocol switching.
    QStringList sessionDate = ui->deSessionDateAndTime->text().split(" ");
    message = WXP::createReadSessionMessage(patient, sessionDate.at(0), sessionDate.at(1));

    emit patientCreated(message);

    this->close();
}

void CreatePatientSessionWindow::showFormErrormMessage(QString field)
{
    QMessageBox::critical(this, "Error", "Please complete the field <b>"+ field + "</b>.");
}



CreatePatientSessionWindow::~CreatePatientSessionWindow()
{
    delete ui;
}
