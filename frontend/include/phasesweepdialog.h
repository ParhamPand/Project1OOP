#ifndef PHASESWEEPDIALOG_H
#define PHASESWEEPDIALOG_H

#include <QDialog>
#include <string>

namespace Ui {
    class PhaseSweepDialog;
}

class PhaseSweepDialog : public QDialog
{
Q_OBJECT

public:
    explicit PhaseSweepDialog(QWidget *parent = nullptr);
    ~PhaseSweepDialog();

    // --- توابع برای گرفتن مقادیر از دیالوگ ---
    double getStartPhase() const;
    double getEndPhase() const;
    int getNumberOfSteps() const;
    std::string getOutputName() const;
    std::string getAnalysisType() const;


private:
    Ui::PhaseSweepDialog *ui;
};

#endif // PHASESWEEPDIALOG_H