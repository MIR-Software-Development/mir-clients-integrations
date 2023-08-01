#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QProcess>
#include "CreatePatientSessionWindow.h"
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
    CreatePatientSessionWindow *patientSessionWindow;
    QTcpSocket *tcpSocketHL7;

    void updateUiForHL7Connection(bool isConnected);
    QString readWspFile(QString fileName);
    void startWXPProtocolWithArgument(QString argument = "/X");
    QString setResultsWspFileStatus(QString fileContent);
private slots:
    void connectToHL7();
    void browseWXPDirectory();
    void createWXPPatient();
    void createHL7Patient();
    void releasePatientWindowMemory();
    void hl7SocketConnected();
    void hl7SocketDisconnected();
    void hl7SocketReadyRead();
    void hl7SocketErrorOccurred(QAbstractSocket::SocketError);
    void processDoneForWXPProtocol(int statusCode, QProcess::ExitStatus status);
    void readPatientSessionWithWXPProtocol();
    void createPatientAndStartWXPProtocol();
    void createWXPPatientSession();
    void releasePatientSessionWindowMemory();

    void updateWXPOperatingSystem(int index);
    void openWXPFVCCurve();
    void openWXPVCCurve();
    void openWXPFVCVtCurve();
    void openWXPFVCPostCurve();
    void openWXPMVVCurve();
    void openWXPOXICurve();
};
#endif // MAINWINDOW_H
