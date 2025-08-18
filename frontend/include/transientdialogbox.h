#ifndef TRANSIENTDIALOGBOX_H
#define TRANSIENTDIALOGBOX_H

#include <QDialog>

namespace Ui {
    class transientDialogBox;
}

class transientDialogBox : public QDialog
{
Q_OBJECT

public:
    explicit transientDialogBox(QWidget *parent = nullptr);
    ~transientDialogBox();

private:
    Ui::transientDialogBox *ui;
};

#endif // TRANSIENTDIALOGBOX_H
