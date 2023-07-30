#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QMessageBox>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // HL7.
    connect(ui->btnHL7Connect, SIGNAL(clicked()), this, SLOT(connectToHL7()));
    connect(ui->pbHL7CreatePatient, SIGNAL(clicked()), this, SLOT(createHL7Patient()));

    // Exchange.
    connect(ui->pbWXPBrowse, SIGNAL(clicked()), this, SLOT(browseWXPDirectory()));
    connect(ui->pbWXPBrowseMIRExe, SIGNAL(clicked()), this, SLOT(browseWXPBrowseMIRExe()));
    connect(ui->pbWXPCreatePatient, SIGNAL(clicked()), this, SLOT(createWXPPatient()));
}

void MainWindow::connectToHL7()
{
    QMessageBox::information(this, "Todo", "To implement.");
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
    patientWindow->open();
}


void MainWindow::createHL7Patient()
{
    patientWindow = new CreatePatientWindow(this, "HL7");
    connect(patientWindow, SIGNAL(finished(int)), this, SLOT(releasePatientWindowMemory()));
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

