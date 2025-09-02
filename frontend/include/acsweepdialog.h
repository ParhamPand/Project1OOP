#ifndef ACSWEEPDIALOG_H
#define ACSWEEPDIALOG_H

#include <QDialog>
#include <string>

namespace Ui {
    class AcSweepDialog;
}

class AcSweepDialog : public QDialog
{
Q_OBJECT

public:
    explicit AcSweepDialog(QWidget *parent = nullptr);
    ~AcSweepDialog();

    std::string getAnalysisType() const;
    double getStartFrequency() const;
    double getEndFrequency() const;
    int getNumberOfSteps() const;
    std::string getOutputName() const;

private:
    Ui::AcSweepDialog *ui;
};

#endif // ACSWEEPDIALOG_H