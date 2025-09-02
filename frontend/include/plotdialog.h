#ifndef PLOTDIALOG_H
#define PLOTDIALOG_H

#include <QDialog>
#include <vector>
#include <utility>

class QCPItemTracer;
class QCPItemText;
class QCPItemLine;
class QMouseEvent;
class QKeyEvent;


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

private slots:
    void onMouseMove(QMouseEvent *event);
    void onMousePress(QMouseEvent *event);


protected:
    void keyPressEvent(QKeyEvent *event) override;

private:

    Ui::PlotDialog *ui;

    QCPItemTracer *m_tracer = nullptr;
    QCPItemText *m_labelText = nullptr;
    QCPItemLine *m_cursorLine = nullptr;

    enum CursorState { Hidden, Tracking, Frozen };
    CursorState m_cursorState = Hidden;

    void setupPlot(const std::vector<std::pair<double, double>>& data,const QString& title, const QString& xAxisLabel, const QString& yAxisLabel);
    void setupCursor();
};

#endif // PLOTDIALOG_H
