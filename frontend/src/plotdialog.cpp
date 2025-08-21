#include "plotdialog.h"
#include "ui_plotdialog.h"
#include "qcustomplot.h"

PlotDialog::PlotDialog(const std::vector<std::pair<double, double>>& data,
                       const QString& title, const QString& xAxisLabel, const QString& yAxisLabel,
                       QWidget *parent) :
        QDialog(parent),
        ui(new Ui::PlotDialog)
{
    ui->setupUi(this);
    setWindowTitle("Analysis Plot");

    setupPlot(data, title, xAxisLabel, yAxisLabel);
}

PlotDialog::~PlotDialog()
{
    delete ui;
}

void PlotDialog::setupPlot(const std::vector<std::pair<double, double>>& data,
                           const QString& title, const QString& xAxisLabel, const QString& yAxisLabel)
{

    QVector<double> x, y;
    for (const auto& pair : data) {
        x.push_back(pair.first);
        y.push_back(pair.second);
    }

    ui->plotWidget->addGraph();
    ui->plotWidget->graph(0)->setData(x, y);


    ui->plotWidget->xAxis->setLabel(xAxisLabel);
    ui->plotWidget->yAxis->setLabel(yAxisLabel);
    ui->plotWidget->plotLayout()->insertRow(0);
    ui->plotWidget->plotLayout()->addElement(0, 0, new QCPTextElement(ui->plotWidget, title, QFont("sans", 12, QFont::Bold)));


    ui->plotWidget->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

    ui->plotWidget->rescaleAxes();

    ui->plotWidget->replot();
}