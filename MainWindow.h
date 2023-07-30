#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "CreatePatientWindow.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    CreatePatientWindow *patientWindow;

private slots:
    void connectToHL7();
    void browseWXPDirectory();
    void browseWXPBrowseMIRExe();
    void createWXPPatient();
    void createHL7Patient();
    void releasePatientWindowMemory();
};
#endif // MAINWINDOW_H
