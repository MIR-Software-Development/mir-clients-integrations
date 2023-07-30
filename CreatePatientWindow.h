#ifndef CREATEPATIENTWINDOW_H
#define CREATEPATIENTWINDOW_H

#include <QDialog>
#include "Patient.h"
#include "HL7.h"
#include "WXP.h"

namespace Ui {
class CreatePatientWindow;
}

class CreatePatientWindow : public QDialog
{
    Q_OBJECT

public:
    explicit CreatePatientWindow(QWidget *parent = nullptr, QString protocol = "");
    ~CreatePatientWindow();
    QString currentProtocol;

private slots:
    void createPatient();

signals:
    void patientCreated(QString);
private:
    Ui::CreatePatientWindow *ui;
};

#endif // CREATEPATIENTWINDOW_H
