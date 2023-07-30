#ifndef CREATEPATIENTWINDOW_H
#define CREATEPATIENTWINDOW_H

#include <QDialog>

namespace Ui {
class CreatePatientWindow;
}

class CreatePatientWindow : public QDialog
{
    Q_OBJECT

public:
    explicit CreatePatientWindow(QWidget *parent = nullptr, QString protocol = "");
    ~CreatePatientWindow();

private:
    Ui::CreatePatientWindow *ui;
};

#endif // CREATEPATIENTWINDOW_H
