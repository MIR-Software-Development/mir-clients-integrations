#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QDesktopServices>

///
/// Init socket and UI components.
/// \brief MainWindow::MainWindow
/// \param parent
///
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    tcpSocketHL7 = NULL;

    // Init UI.
    this->updateUiForHL7Connection(false);
    this->updateWXPOperatingSystem(0);

    // HL7.
    connect(ui->pbHL7Connect, SIGNAL(clicked()), this, SLOT(connectToHL7()));
    connect(ui->pbHL7CreatePatient, SIGNAL(clicked()), this, SLOT(createHL7Patient()));

    // Exchange.
    connect(ui->pbWXPBrowse, SIGNAL(clicked()), this, SLOT(browseWXPDirectory()));
    connect(ui->cbWXPSelectOS, SIGNAL(currentIndexChanged(int)), this, SLOT(updateWXPOperatingSystem(int)));
    connect(ui->pbWXPCreatePatient, SIGNAL(clicked()), this, SLOT(createWXPPatient()));
    connect(ui->pbWXPCreatePatientSession, SIGNAL(clicked()), this, SLOT(createWXPPatientSession()));
    connect(ui->pbWXPSendPatient, SIGNAL(clicked()), this, SLOT(createPatientAndStartWXPProtocol()));
    connect(ui->pbWXPReadPatientSession, SIGNAL(clicked()), this, SLOT(readPatientSessionWithWXPProtocol()));

    connect(ui->pbWXPCurvesFVC, SIGNAL(clicked()), this, SLOT(openWXPFVCCurve()));
    connect(ui->pbWXPCurvesVC, SIGNAL(clicked()), this, SLOT(openWXPVCCurve()));
    connect(ui->pbWXPCurvesFVCVT, SIGNAL(clicked()), this, SLOT(openWXPFVCVtCurve()));
    connect(ui->pbWXPCurvesFVCPOST, SIGNAL(clicked()), this, SLOT(openWXPFVCPostCurve()));
    connect(ui->pbWXPCurvesMVV, SIGNAL(clicked()), this, SLOT(openWXPMVVCurve()));
    connect(ui->pbWXPCurvesOXI, SIGNAL(clicked()), this, SLOT(openWXPOXICurve()));
}

///
/// When user tries to establish a connection with MIR app through TCP Socket.
/// \brief MainWindow::connectToHL7
///
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

///
/// Event triggered when the TCP socket is connected.
/// \brief MainWindow::hl7SocketConnected
///
void MainWindow::hl7SocketConnected()
{
    qDebug() << "Connected";

    this->updateUiForHL7Connection(true);
}

///
/// Event triggered when the TCP socket is disconnected.
/// \brief MainWindow::hl7SocketDisconnected
///
void MainWindow::hl7SocketDisconnected()
{
    qDebug() << "Disconnected";

    this->updateUiForHL7Connection(false);
    delete tcpSocketHL7;
}

///
/// Event triggered when the TCP socket is ready to receive new messages.
/// \brief MainWindow::hl7SocketReadyRead
///
void MainWindow::hl7SocketReadyRead()
{
    qDebug() << "Ready...";

    // TODO: Add messages received in the plain text field.
}

///
/// Event triggered when the TCP socket has an error.
/// \brief MainWindow::hl7SocketErrorOccurred
/// \param error
///
void MainWindow::hl7SocketErrorOccurred(QAbstractSocket::SocketError error)
{
    QMessageBox::critical(this, "Connection refused", "An error occurred during the connection:\n" + tcpSocketHL7->errorString());

    delete tcpSocketHL7;
}

///
/// Update the UI based on the HL7 connection status.
/// \brief MainWindow::updateUiForHL7Connection
/// \param isConnected
///
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

///
/// Open the window to create the patient HL7 string.
/// \brief MainWindow::createHL7Patient
///
void MainWindow::createHL7Patient()
{
    patientWindow = new CreatePatientWindow(this, "HL7");
    connect(patientWindow, SIGNAL(finished(int)), this, SLOT(releasePatientWindowMemory()));
    connect(patientWindow, SIGNAL(patientCreated(QString)), ui->teHL7MsgToSend, SLOT(setText(QString)));
    patientWindow->open();
}

///
/// Open the OS prompt to select the Exchange directory.
/// \brief MainWindow::browseWXPDirectory
///
void MainWindow::browseWXPDirectory()
{
    QString directory = QFileDialog::getExistingDirectory(this, "Select MIR directory...", "C:/", QFileDialog::ShowDirsOnly);

    ui->leWXPMIRAppPath->setText(directory);
}

void MainWindow::updateWXPOperatingSystem(int index)
{
    if (index == 0) {
        qDebug() << "Windows";

        QStringList operatingSystems;
        operatingSystems << "C:/MIR/WinspiroPRO/winspiroPRO.exe" <<
            "C:/MIR/spiroCONNECT/spiroCONNECT.exe" <<
            "C:/MIR/MIR Spiro/MIR Spiro.exe";

        ui->lWXPMIRDirectoryPath->setText("Directory of MIR application");
        ui->cbWXPMIRExecutable->clear();
        ui->cbWXPMIRExecutable->addItems(operatingSystems);

        return;
    }


    ui->lWXPMIRDirectoryPath->setText("Directory of Exchange files");
    ui->cbWXPMIRExecutable->clear();
    ui->cbWXPMIRExecutable->addItems(QStringList("MIR Spiro"));

    qDebug() << "MacOS";
}

///
/// Open the window to create the patient.SRV content file to create a patient and start tests.
/// \brief MainWindow::createWXPPatient
///
void MainWindow::createWXPPatient()
{
    patientWindow = new CreatePatientWindow(this, "WXP");
    connect(patientWindow, SIGNAL(finished(int)), this, SLOT(releasePatientWindowMemory()));
    connect(patientWindow, SIGNAL(patientCreated(QString)), ui->teWXPPatientSRVContent, SLOT(setText(QString)));
    patientWindow->open();
}

///
/// Open the window to create the patient.SRV content file to consult a session.
/// \brief MainWindow::createWXPPatientSession
///
void MainWindow::createWXPPatientSession()
{
    patientSessionWindow = new CreatePatientSessionWindow(this);
    connect(patientSessionWindow, SIGNAL(finished(int)), this, SLOT(releasePatientSessionWindowMemory()));
    connect(patientSessionWindow, SIGNAL(patientCreated(QString)), ui->teWXPPatientSRVContent, SLOT(setText(QString)));

    patientSessionWindow->open();
}

///
/// Start the MIR app with /X argument.
/// \brief MainWindow::createPatientAndStartWXPProtocol
///
void MainWindow::createPatientAndStartWXPProtocol()
{
    this->startWXPProtocolWithArgument("/X");
}

///
/// Start the MIR app with /V argument.
/// \brief MainWindow::readPatientSessionWithWXPProtocol
///
void MainWindow::readPatientSessionWithWXPProtocol()
{
    this->startWXPProtocolWithArgument("/V");
}

///
/// Start the MIR app with the argument specified.
/// \brief MainWindow::startWXPProtocolWithArgument
/// \param argument
///
void MainWindow::startWXPProtocolWithArgument(QString argument)
{
    if (ui->leWXPMIRAppPath->text().isEmpty()) {
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

        process->start(ui->cbWXPMIRExecutable->currentText(), QStringList(argument));

        return;
    }

    QMessageBox::critical(this, "Error", "Cannot create the file or start the executable file. Please make sure you have the permissions.");
}

///
/// When MIR app is closed, it reads the Exchange protocol files generated to complete the fields.
/// \brief MainWindow::processDoneForWXPProtocol
/// \param statusCode
/// \param status
///
void MainWindow::processDoneForWXPProtocol(int statusCode, QProcess::ExitStatus status)
{
    QString content = NULL;
    // Results.wsp
    ui->teWXPResultsWspFile->setText(content = this->readWspFile(ui->leWXPMIRAppPath->text() + "/Results.wsp"));
    ui->lWXPResultsWspFile->setText(this->setResultsWspFileStatus(content));

    // Results2ndBest.wsp
    ui->teWXPResults2ndBestFile->setText(content = this->readWspFile(ui->leWXPMIRAppPath->text() + "/Results2ndBest.wsp"));
    ui->lWXPResults2ndBestFile->setText(this->setResultsWspFileStatus(content));

    // Results3rdBest.wsp
    ui->teWXPResults3rdBestFile->setText(content = this->readWspFile(ui->leWXPMIRAppPath->text() + "/Results3rdBest.wsp"));
    ui->lWXPResults3rdBestFile->setText(this->setResultsWspFileStatus(content));

    // ResultsPostBd.wsp
    ui->teWXPResultsPostBdFile->setText(content = this->readWspFile(ui->leWXPMIRAppPath->text() + "/ResultsPostBd.wsp"));
    ui->lWXPResultsPostBdFile->setText(this->setResultsWspFileStatus(content));

    // ResultsOxi.wsp
    ui->teWXPResultsOxiFile->setText(content = this->readWspFile(ui->leWXPMIRAppPath->text() + "/ResultsOxi.wsp"));
    ui->lWXPResultsOxiFile->setText(this->setResultsWspFileStatus(content));

    // SpiroResults.wsp
    ui->teWXPResultsSpiroResultsFile->setText(content = this->readWspFile(ui->leWXPMIRAppPath->text() + "/SpiroResults.wsp"));
    ui->lWXPResultsSpiroResultsFile->setText(this->setResultsWspFileStatus(content));

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

///
/// Read the .wsp files from MIR directory.
/// \brief MainWindow::readWspFile
/// \param fileName
/// \return
///
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

///
/// Check if the file contains \r\n and \t chars.
/// \brief MainWindow::setResultsWspFileStatus
/// \param fileContent
/// \return
///
QString MainWindow::setResultsWspFileStatus(QString fileContent)
{
    if (!fileContent.contains("\n") || !fileContent.contains("\t")) {
        return "<span style=\"color: red; font-weight: bold;\">This file is not valid!</span>";
    }

    return "<span style=\"color: green; font-weight: bold;\">This file is valid</span>";
}

///
/// Open the FVC.jpg file.
/// \brief MainWindow::openWXPFVCCurve
///
void MainWindow::openWXPFVCCurve()
{
    QDesktopServices::openUrl(QUrl(ui->leWXPCurvesFVC->text()));
}

///
/// Open the VC.jpg file.
/// \brief MainWindow::openWXPVCCurve
///
void MainWindow::openWXPVCCurve()
{
    QDesktopServices::openUrl(QUrl(ui->leWXPCurvesVC->text()));
}

///
/// Open the FVC_VT.jpg file.
/// \brief MainWindow::openWXPFVCVtCurve
///
void MainWindow::openWXPFVCVtCurve()
{
    QDesktopServices::openUrl(QUrl(ui->leWXPCurvesFVCVT->text()));
}

///
/// Open the FVC_POST.jpg file.
/// \brief MainWindow::openWXPFVCPostCurve
///
void MainWindow::openWXPFVCPostCurve()
{
    QDesktopServices::openUrl(QUrl(ui->leWXPCurvesFVCPOST->text()));
}

///
/// Open the MVV.jpg file.
/// \brief MainWindow::openWXPMVVCurve
///
void MainWindow::openWXPMVVCurve()
{
    QDesktopServices::openUrl(QUrl(ui->leWXPCurvesMVV->text()));
}

///
/// Open the OXI.jpg file.
/// \brief MainWindow::openWXPOXICurve
///
void MainWindow::openWXPOXICurve()
{
    QDesktopServices::openUrl(QUrl(ui->leWXPCurvesOXI->text()));
}

///
/// Delete the pointer to release memory.
/// \brief MainWindow::releasePatientWindowMemory
///
void MainWindow::releasePatientWindowMemory()
{
    delete patientWindow;
}

///
/// Delete the pointer to release memory.
/// \brief MainWindow::releasePatientSessionWindowMemory
///
void MainWindow::releasePatientSessionWindowMemory()
{
    //delete patientSessionWindow;
}

MainWindow::~MainWindow()
{
    delete ui;
}

