#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QMessageBox>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    tcpSocketHL7 = NULL;

    // Init UI.
    this->updateUiForHL7Connection(false);

    // HL7.
    connect(ui->pbHL7Connect, SIGNAL(clicked()), this, SLOT(connectToHL7()));
    connect(ui->pbHL7CreatePatient, SIGNAL(clicked()), this, SLOT(createHL7Patient()));

    // Exchange.
    connect(ui->pbWXPBrowse, SIGNAL(clicked()), this, SLOT(browseWXPDirectory()));
    connect(ui->pbWXPBrowseMIRExe, SIGNAL(clicked()), this, SLOT(browseWXPBrowseMIRExe()));
    connect(ui->pbWXPCreatePatient, SIGNAL(clicked()), this, SLOT(createWXPPatient()));
    connect(ui->pbWXPCreatePatientSession, SIGNAL(clicked()), this, SLOT(createWXPPatientSession()));
    connect(ui->pbWXPSendPatient, SIGNAL(clicked()), this, SLOT(createPatientAndStartWXPProtocol()));
    connect(ui->pbWXPReadPatientSession, SIGNAL(clicked()), this, SLOT(readPatientSessionWithWXPProtocol()));
}

void MainWindow::connectToHL7()
{
    if (tcpSocketHL7 != NULL && tcpSocketHL7->isOpen()) {
        tcpSocketHL7->disconnect();
        return;
    }

    tcpSocketHL7 = new QTcpSocket(this);

    connect(tcpSocketHL7, SIGNAL(connected()), this, SLOT(hl7SocketConnected()));
    connect(tcpSocketHL7, SIGNAL(disconnected()), this, SLOT(hl7SocketDisconnected()));
    connect(tcpSocketHL7, SIGNAL(readyRead()), this, SLOT(hl7SocketReadyRead()));
    connect(tcpSocketHL7, SIGNAL(errorOccurred(QAbstractSocket::SocketError)), this, SLOT(hl7SocketErrorOccurred(QAbstractSocket::SocketError)));

    tcpSocketHL7->connectToHost(ui->leHL7IpAddress->text(), ui->sbHL7Port->text().toInt());
}

void MainWindow::hl7SocketConnected()
{
    qDebug() << "Connected";
}

void MainWindow::hl7SocketDisconnected()
{
    qDebug() << "Disconnected";

    this->updateUiForHL7Connection(false);
    delete tcpSocketHL7;
}

void MainWindow::hl7SocketReadyRead()
{
    qDebug() << "Ready...";

    this->updateUiForHL7Connection(true);
}

void MainWindow::hl7SocketErrorOccurred(QAbstractSocket::SocketError error)
{
    QMessageBox::critical(this, "Connection refused", "An error occurred during the connection:\n" + tcpSocketHL7->errorString());

    delete tcpSocketHL7;
}

void MainWindow::updateUiForHL7Connection(bool isConnected = true)
{
    QString status = "<span style='color: {color};font-weight: bold'>{status}</span>";

    ui->leHL7IpAddress->setEnabled(!isConnected);
    ui->sbHL7Port->setEnabled(!isConnected);

    ui->pbHL7Connect->setText(isConnected ? "Disconnect": "Connect");
    ui->lHL7SocketStatus->setText(status.replace("{color}", isConnected ? "green": "red").replace("{status}", isConnected ? "Connected": "Disconnected"));

    ui->lHL7MsgToSend->setVisible(isConnected);
    ui->lHL7MessageReceived->setVisible(isConnected);
    ui->lHL7QuickOperations->setVisible(isConnected);
    ui->pbHL7SendMessage->setVisible(isConnected);
    ui->pbHL7CreatePatient->setVisible(isConnected);
    ui->pbHL7ReadSession->setVisible(isConnected);
    ui->teHL7MsgToSend->setVisible(isConnected);
    ui->teHL7MessageReceived->setVisible(isConnected);
}

void MainWindow::createHL7Patient()
{
    patientWindow = new CreatePatientWindow(this, "HL7");
    connect(patientWindow, SIGNAL(finished(int)), this, SLOT(releasePatientWindowMemory()));
    connect(patientWindow, SIGNAL(patientCreated(QString)), ui->teHL7MsgToSend, SLOT(setText(QString)));
    patientWindow->open();
}

void MainWindow::browseWXPDirectory()
{
    QString directory = QFileDialog::getExistingDirectory(this, "Select MIR directory...", "C:/", QFileDialog::ShowDirsOnly);

    ui->leWXPMIRAppPath->setText(directory);
}

void MainWindow::browseWXPBrowseMIRExe()
{
    QString exeFile = QFileDialog::getOpenFileName(this, "Select MIR executable file...", "C:/MIR/", "*.exe");

    ui->leWXPMIRExePath->setText(exeFile);
}

void MainWindow::createWXPPatient()
{
    patientWindow = new CreatePatientWindow(this, "WXP");
    connect(patientWindow, SIGNAL(finished(int)), this, SLOT(releasePatientWindowMemory()));
    connect(patientWindow, SIGNAL(patientCreated(QString)), ui->teWXPPatientSRVContent, SLOT(setText(QString)));
    patientWindow->open();
}

void MainWindow::createWXPPatientSession()
{
    patientSessionWindow = new CreatePatientSessionWindow(this);
    connect(patientSessionWindow, SIGNAL(finished(int)), this, SLOT(releasePatientSessionWindowMemory()));
    connect(patientSessionWindow, SIGNAL(patientCreated(QString)), ui->teWXPPatientSRVContent, SLOT(setText(QString)));

    patientSessionWindow->open();
}

void MainWindow::createPatientAndStartWXPProtocol()
{
    this->startWXPProtocolWithArgument("/X");
}

void MainWindow::readPatientSessionWithWXPProtocol()
{
    this->startWXPProtocolWithArgument("/V");
}

void MainWindow::startWXPProtocolWithArgument(QString argument)
{
    if (ui->leWXPMIRAppPath->text().isEmpty() || ui->leWXPMIRExePath->text().isEmpty()) {
        QMessageBox::critical(this, "Error", "The MIR directory and the executable MIR path must be filled.");
        return;
    }

    if (ui->teWXPPatientSRVContent->toPlainText().isEmpty()) {
        QMessageBox::critical(this, "Error", "The content of the Patient.SRV file cannot be empty.");
        return;
    }

    QFile file(ui->leWXPMIRAppPath->text() + "/Patient.SRV");
    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);
        stream << ui->teWXPPatientSRVContent->toPlainText() << Qt::endl;

        file.close();

        QProcess *process = new QProcess(this);
        connect(process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(processDoneForWXPProtocol(int, QProcess::ExitStatus)));

        process->start(ui->leWXPMIRExePath->text(), QStringList(argument));

        return;
    }

    QMessageBox::critical(this, "Error", "Cannot create the file or start the executable file. Please make sure you have the permissions.");
}

void MainWindow::processDoneForWXPProtocol(int statusCode, QProcess::ExitStatus status)
{
    // Results.wsp
    ui->teWXPResultsWspFile->setText(this->readWspFile(ui->leWXPMIRAppPath->text() + "/Results.wsp"));

    // Results2ndBest.wsp
    ui->teWXPResults2ndBestFile->setText(this->readWspFile(ui->leWXPMIRAppPath->text() + "/Results2ndBest.wsp"));

    // Results3rdBest.wsp
    ui->teWXPResults3rdBestFile->setText(this->readWspFile(ui->leWXPMIRAppPath->text() + "/Results3rdBest.wsp"));

    // ResultsPostBd.wsp
    ui->teWXPResultsPostBdFile->setText(this->readWspFile(ui->leWXPMIRAppPath->text() + "/ResultsPostBd.wsp"));

    // ResultsOxi.wsp
    ui->teWXPResultsOxiFile->setText(this->readWspFile(ui->leWXPMIRAppPath->text() + "/ResultsOxi.wsp"));

    // SpiroResults.wsp
    ui->teWXPResultsSpiroResultsFile->setText(this->readWspFile(ui->leWXPMIRAppPath->text() + "/SpiroResults.wsp"));

    // Then, adds the PDF file if it exists.
    if (QFile(ui->leWXPMIRAppPath->text() + "/SpiroResults.pdf").exists()) {
        ui->leWXPResultsSpiroResultsPdf->setText(ui->leWXPMIRAppPath->text() + "/SpiroResults.pdf");
    }

    // Now it checks if the curves exists.
    if (QFile(ui->leWXPMIRAppPath->text() + "/FVC.jpg").exists()) {
        ui->leWXPCurvesFVC->setText(ui->leWXPMIRAppPath->text() + "/FVC.jpg");
    }

    if (QFile(ui->leWXPMIRAppPath->text() + "/VC.jpg").exists()) {
        ui->leWXPCurvesVC->setText(ui->leWXPMIRAppPath->text() + "/VC.jpg");
    }

    if (QFile(ui->leWXPMIRAppPath->text() + "/FVC_POST.jpg").exists()) {
        ui->leWXPCurvesFVCPOST->setText(ui->leWXPMIRAppPath->text() + "/FVC_POST.jpg");
    }

    if (QFile(ui->leWXPMIRAppPath->text() + "/MVV.jpg").exists()) {
        ui->leWXPCurvesMVV->setText(ui->leWXPMIRAppPath->text() + "/MVV.jpg");
    }

    if (QFile(ui->leWXPMIRAppPath->text() + "/OXI.jpg").exists()) {
        ui->leWXPCurvesOXI->setText(ui->leWXPMIRAppPath->text() + "/OXI.jpg");
    }
}

QString MainWindow::readWspFile(QString fileName)
{
    QFile file(fileName);
    if (!file.exists()) {
        return NULL;
    }

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return NULL;
    }

    QTextStream in(&file);
    QString fileContent = in.readAll();

    file.close();

    return fileContent;
}


void MainWindow::releasePatientWindowMemory()
{
    delete patientWindow;
}

void MainWindow::releasePatientSessionWindowMemory()
{
    //delete patientSessionWindow;
}


MainWindow::~MainWindow()
{
    delete ui;
}

