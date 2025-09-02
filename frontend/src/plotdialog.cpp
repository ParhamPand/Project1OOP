#include "plotdialog.h"
#include "ui_plotdialog.h"
#include "qcustomplot.h"
#include <QKeyEvent>

PlotDialog::PlotDialog(const std::vector<std::pair<double, double>>& data,
                       const QString& title, const QString& xAxisLabel, const QString& yAxisLabel,
                       QWidget *parent) :
        QDialog(parent),
        ui(new Ui::PlotDialog)
{
    ui->setupUi(this);
    setWindowTitle("Analysis Plot");

    setupPlot(data, title, xAxisLabel, yAxisLabel);
    setupCursor();

    connect(ui->plotWidget, &QCustomPlot::mousePress, this, &PlotDialog::onMousePress);
}

PlotDialog::~PlotDialog()
{
    delete ui;
}

void PlotDialog::setupCursor()
{
    m_tracer = new QCPItemTracer(ui->plotWidget);
    m_tracer->setGraph(ui->plotWidget->graph(0));
    m_tracer->setStyle(QCPItemTracer::tsCircle);
    m_tracer->setPen(QPen(Qt::red, 1.5));
    m_tracer->setBrush(Qt::red);
    m_tracer->setSize(7);
    m_tracer->setVisible(false);

    m_cursorLine = new QCPItemLine(ui->plotWidget);
    m_cursorLine->setPen(QPen(Qt::gray, 1, Qt::DashLine));
    m_cursorLine->setVisible(false);


    m_labelText = new QCPItemText(ui->plotWidget);
    m_labelText->setPadding(QMargins(5, 5, 5, 5));
    m_labelText->setBrush(QBrush(QColor(240, 240, 240, 200)));
    m_labelText->setPen(QPen(Qt::gray));
    m_labelText->position->setParentAnchor(m_tracer->position);
    m_labelText->position->setCoords(0, -30);
    m_labelText->setFont(QFont("Arial", 10));
    m_labelText->setVisible(false);
}

void PlotDialog::onMouseMove(QMouseEvent *event)
{
    if (m_cursorState != Tracking) return;

    double mouseX = ui->plotWidget->xAxis->pixelToCoord(event->pos().x());


    m_tracer->setGraphKey(mouseX);
    m_tracer->updatePosition();


    m_cursorLine->start->setCoords(m_tracer->position->key(), ui->plotWidget->yAxis->range().lower);
    m_cursorLine->end->setCoords(m_tracer->position->key(), ui->plotWidget->yAxis->range().upper);

    double xVal = m_tracer->position->key();
    double yVal = m_tracer->position->value();
    m_labelText->setText(QString("(%1, %2)").arg(xVal, 0, 'f', 2).arg(yVal, 0, 'f', 2));

    ui->plotWidget->replot();
}

void PlotDialog::onMousePress(QMouseEvent *event)
{
    if (m_cursorState == Tracking) {
        m_cursorState = Frozen;
        disconnect(ui->plotWidget, &QCustomPlot::mouseMove, this, &PlotDialog::onMouseMove);
    }
}

void PlotDialog::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_C) {
        if (m_cursorState == Hidden || m_cursorState == Frozen) {
            m_cursorState = Tracking;
            m_tracer->setVisible(true);

            m_cursorLine->setVisible(true);
            m_labelText->setVisible(true);

            connect(ui->plotWidget, &QCustomPlot::mouseMove, this, &PlotDialog::onMouseMove);
        } else if (m_cursorState == Tracking) {
            m_cursorState = Hidden;
            m_tracer->setVisible(false);
            m_cursorLine->setVisible(false);
            m_labelText->setVisible(false);
            disconnect(ui->plotWidget, &QCustomPlot::mouseMove, this, &PlotDialog::onMouseMove);
            ui->plotWidget->replot();
        }
    } else {
        QDialog::keyPressEvent(event);
    }
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

    this->setFocus();
}