#include "sinusoidalsourcedialog.h"
#include "ui_sinusoidalsourcedialog.h"

SinusoidalSourceDialog::SinusoidalSourceDialog(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::SinusoidalSourceDialog)
{
    ui->setupUi(this);
    setWindowTitle("Sinusoidal Source Properties");
}

SinusoidalSourceDialog::~SinusoidalSourceDialog()
{
    delete ui;
}

void SinusoidalSourceDialog::setOffset(double offset) { ui->offsetLineEdit->setText(QString::number(offset)); }
void SinusoidalSourceDialog::setAmplitude(double amplitude) { ui->amplitudeLineEdit->setText(QString::number(amplitude)); }
void SinusoidalSourceDialog::setFrequency(double frequency) { ui->frequencyLineEdit->setText(QString::number(frequency)); }
void SinusoidalSourceDialog::setPhase(double phase) { ui->phaseLineEdit->setText(QString::number(phase)); }

double SinusoidalSourceDialog::getOffset() const { return ui->offsetLineEdit->text().toDouble(); }
double SinusoidalSourceDialog::getAmplitude() const { return ui->amplitudeLineEdit->text().toDouble(); }
double SinusoidalSourceDialog::getFrequency() const { return ui->frequencyLineEdit->text().toDouble(); }
double SinusoidalSourceDialog::getPhase() const { return ui->phaseLineEdit->text().toDouble(); }