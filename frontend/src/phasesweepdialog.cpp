#include "phasesweepdialog.h"
#include "ui_phasesweepdialog.h"

PhaseSweepDialog::PhaseSweepDialog(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::PhaseSweepDialog)
{
    ui->setupUi(this);
    setWindowTitle("Phase Sweep Analysis");
}

PhaseSweepDialog::~PhaseSweepDialog()
{
    delete ui;
}

double PhaseSweepDialog::getStartPhase() const
{
    return ui->startPhaseLineEdit->text().toDouble();
}

double PhaseSweepDialog::getEndPhase() const
{
    return ui->endPhaseLineEdit->text().toDouble();
}

int PhaseSweepDialog::getNumberOfSteps() const
{
    return ui->numStepsLineEdit->text().toInt();
}

std::string PhaseSweepDialog::getAnalysisType() const
{
    return ui->analysisTypeComboBox->currentText().toStdString();
}


std::string PhaseSweepDialog::getOutputName() const
{
    std::string type = getAnalysisType().substr(0, 1);
    std::string name = ui->outputNameLineEdit->text().toStdString();
    return type + "(" + name + ")";
}