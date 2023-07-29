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

    // Exchange.
    connect(ui->pbWXPBrowse, SIGNAL(clicked()), this, SLOT(browserWXPDirectory()));
}

void MainWindow::connectToHL7()
{
    QMessageBox::information(this, "Todo", "To implement.");
}

void MainWindow::browserWXPDirectory()
{
    QString directory = QFileDialog::getExistingDirectory(this, "Select MIR directory...", "C:/", QFileDialog::ShowDirsOnly);

    ui->leWXPMIRAppPath->setText(directory);
}

MainWindow::~MainWindow()
{
    delete ui;
}

