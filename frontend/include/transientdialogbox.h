#ifndef TRANSIENTDIALOGBOX_H
#define TRANSIENTDIALOGBOX_H

#include <QDialog>
#include <string>

namespace Ui {
    class transientDialogBox;
}

class transientDialogBox : public QDialog
{
Q_OBJECT

public:
    explicit transientDialogBox(QWidget *parent = nullptr);
    ~transientDialogBox();

    std::string getAnalysisType() const;
    double getStartTime() const;
    double getTimeStep() const;
    double getStopTime() const;
    std::string getOutputName() const;

private:
    Ui::transientDialogBox *ui;
};

#endif // TRANSIENTDIALOGBOX_H