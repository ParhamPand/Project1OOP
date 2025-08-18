#include "acsweepdialog.h"
#include "ui_acsweepdialog.h"

AcSweepDialog::AcSweepDialog(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::AcSweepDialog)
{
    ui->setupUi(this);
}

AcSweepDialog::~AcSweepDialog()
{
    delete ui;
}
