#ifndef CREATEPATIENTSESSIONWINDOW_H
#define CREATEPATIENTSESSIONWINDOW_H

#include <QDialog>
#include "Patient.h"
#include "WXP.h"

namespace Ui {
class CreatePatientSessionWindow;
}

class CreatePatientSessionWindow : public QDialog
{
    Q_OBJECT

public:
    explicit CreatePatientSessionWindow(QWidget *parent = nullptr);
    ~CreatePatientSessionWindow();

private slots:
    void createSession();
private:
    Ui::CreatePatientSessionWindow *ui;
    void showFormErrormMessage(QString field);

signals:
    void patientCreated(QString);
};

#endif // CREATEPATIENTSESSIONWINDOW_H
