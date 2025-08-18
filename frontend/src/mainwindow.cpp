#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "transientdialogbox.h"
#include "acsweepdialog.h"
#include "phasesweepdialog.h"
#include "nodelibrarydialog.h"
#include <QToolButton>
#include <QMenu>

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow) {

    ui->setupUi(this);

    {
        QMenu *voltageMenu = new QMenu(this);
        voltageMenu->addAction(ui->actionAddVdc);
        voltageMenu->addAction(ui->actionAddVsin);
        QToolButton *voltageButton = new QToolButton(this);
        voltageButton->setText("Voltage Source");
        voltageButton->setMenu(voltageMenu);
        voltageButton->setPopupMode(QToolButton::MenuButtonPopup);
        ui->toolBar->addWidget(voltageButton);

    }
    {
        QMenu *currentMenu = new QMenu(this);
        currentMenu->addAction(ui->actionAddIdc);
        currentMenu->addAction(ui->actionAddIsin);
        QToolButton *currentButton = new QToolButton(this);
        currentButton->setText("Current Source");
        currentButton->setMenu(currentMenu);
        currentButton->setPopupMode(QToolButton::MenuButtonPopup);
        ui->toolBar->addWidget(currentButton);
    }

    connect(ui->actionTransient, &QAction::triggered, this, &MainWindow::on_actionTransient_triggered);
    connect(ui->actionAcSweep, &QAction::triggered, this, &MainWindow::on_actionAcSweep_triggered);
    connect(ui->actionPhaseSweep, &QAction::triggered, this, &MainWindow::on_actionPhaseSweep_triggered);

}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_actionTransient_triggered()
{
    transientDialogBox dialog(this);
    dialog.exec();
}

void MainWindow::on_actionAcSweep_triggered()
{
    AcSweepDialog dialog(this);
    dialog.exec();
}

void MainWindow::on_actionPhaseSweep_triggered()
{
    PhaseSweepDialog dialog(this);
    dialog.exec();
}

void MainWindow::on_actionOpenNodeLibrary_triggered()
{
    NodeLibraryDialog dialog(this);

    // 1. Check if the user made a choice
    if (dialog.exec() == QDialog::Accepted)
    {
        // 2. Get the specific choice from the dialog
        std::string selected = dialog.getSelectedComponent();
        qDebug() << "User selected:" << QString::fromStdString(selected);
    }
}