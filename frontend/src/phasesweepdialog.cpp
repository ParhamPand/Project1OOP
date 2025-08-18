#include "phasesweepdialog.h"
#include "ui_phasesweepdialog.h"

PhaseSweepDialog::PhaseSweepDialog(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::PhaseSweepDialog)
{
    ui->setupUi(this);
}

PhaseSweepDialog::~PhaseSweepDialog()
{
    delete ui;
}