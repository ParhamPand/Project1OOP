#include "acsweepdialog.h"
#include "ui_acsweepdialog.h"

AcSweepDialog::AcSweepDialog(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::AcSweepDialog)
{
    ui->setupUi(this);
    setWindowTitle("AC Sweep Analysis");
}

AcSweepDialog::~AcSweepDialog()
{
    delete ui;
}



std::string AcSweepDialog::getAnalysisType() const
{
    return ui->analysisTypeComboBox->currentText().toStdString();
}

double AcSweepDialog::getStartFrequency() const
{
    return ui->startFreqLineEdit->text().toDouble();
}

double AcSweepDialog::getEndFrequency() const
{
    return ui->endFreqLineEdit->text().toDouble();
}

int AcSweepDialog::getNumberOfSteps() const
{
    return ui->numStepsLineEdit->text().toInt();
}

std::string AcSweepDialog::getOutputName() const
{

    std::string type = getAnalysisType().substr(0, 1);
    std::string name = ui->outputNameLineEdit->text().toStdString();
    return type + "(" + name + ")";
}