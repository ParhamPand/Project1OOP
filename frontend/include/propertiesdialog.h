#ifndef PROPERTIESDIALOG_H
#define PROPERTIESDIALOG_H

#include <QDialog>
#include <string>

namespace Ui {
    class PropertiesDialog;
}

class PropertiesDialog : public QDialog
{
Q_OBJECT

public:
    explicit PropertiesDialog(QWidget *parent = nullptr);
    ~PropertiesDialog();

    void setComponentName(const QString &name);
    void setCurrentValue(double value);
    QString getNewValueText() const;

private:
    Ui::PropertiesDialog *ui;
};

#endif // PROPERTIESDIALOG_H