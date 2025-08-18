#ifndef ACSWEEPDIALOG_H
#define ACSWEEPDIALOG_H

#include <QDialog>

namespace Ui {
    class AcSweepDialog;
}

class AcSweepDialog : public QDialog
{
Q_OBJECT

public:
    explicit AcSweepDialog(QWidget *parent = nullptr);
    ~AcSweepDialog();

private:
    Ui::AcSweepDialog *ui;
};

#endif