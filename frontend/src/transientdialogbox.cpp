#include "transientdialogbox.h"
#include "ui_transientdialogbox.h"

transientDialogBox::transientDialogBox(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::transientDialogBox)
{
    ui->setupUi(this);
}

transientDialogBox::~transientDialogBox()
{
    delete ui;
}