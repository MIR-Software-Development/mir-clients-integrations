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

void MainWindow::releasePatientWindowMemory()
{
    delete patientWindow;
}

MainWindow::~MainWindow()
{
    delete ui;
}

