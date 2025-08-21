#ifndef PLOTDIALOG_H
#define PLOTDIALOG_H

#include <QDialog>
#include <vector>
#include <utility>

namespace Ui {
    class PlotDialog;
}

class PlotDialog : public QDialog
{
Q_OBJECT

public:
    explicit PlotDialog(const std::vector<std::pair<double, double>>& data,
                        const QString& title, const QString& xAxisLabel, const QString& yAxisLabel,
                        QWidget *parent = nullptr);
    ~PlotDialog();

private:
    Ui::PlotDialog *ui;
    void setupPlot(const std::vector<std::pair<double, double>>& data,
                   const QString& title, const QString& xAxisLabel, const QString& yAxisLabel);
};

#endif // PLOTDIALOG_H
