#include "propertiesdialog.h"
#include "ui_propertiesdialog.h"

PropertiesDialog::PropertiesDialog(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::PropertiesDialog)
{
    ui->setupUi(this);
    setWindowTitle("Component Properties");
}

PropertiesDialog::~PropertiesDialog()
{
    delete ui;
}

void PropertiesDialog::setComponentName(const QString &name)
{
    ui->nameLabel->setText(name);
}

void PropertiesDialog::setCurrentValue(double value)
{
    ui->valueLineEdit->setText(QString::number(value));
}

QString PropertiesDialog::getNewValueText() const
{
    return ui->valueLineEdit->text();
}