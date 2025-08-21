#include "transientdialogbox.h"
#include "ui_transientdialogbox.h"

transientDialogBox::transientDialogBox(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::transientDialogBox)
{
    ui->setupUi(this);
    setWindowTitle("Transient Analysis");
}

transientDialogBox::~transientDialogBox()
{
    delete ui;
}

std::string transientDialogBox::getAnalysisType() const
{
    return ui->analysisTypeComboBox->currentText().toStdString();
}

double transientDialogBox::getStartTime() const
{
    return ui->startTimeLineEdit->text().toDouble();
}

double transientDialogBox::getTimeStep() const
{
    return ui->timeStepLineEdit->text().toDouble();
}

double transientDialogBox::getStopTime() const
{
    return ui->stopTimeLineEdit->text().toDouble();
}

std::string transientDialogBox::getOutputName() const
{
    return ui->outputNameLineEdit->text().toStdString();
}