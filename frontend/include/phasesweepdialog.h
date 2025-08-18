#ifndef PHASESWEEPDIALOG_H
#define PHASESWEEPDIALOG_H

#include <QDialog>

namespace Ui {
    class PhaseSweepDialog;
}

class PhaseSweepDialog : public QDialog
{
Q_OBJECT

public:
    explicit PhaseSweepDialog(QWidget *parent = nullptr);
    ~PhaseSweepDialog();

private:
    Ui::PhaseSweepDialog *ui;
};

#endif // PHASESWEEPDIALOG_H