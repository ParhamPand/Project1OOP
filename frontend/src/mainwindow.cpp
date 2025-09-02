#include <string>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Resistor.h"
#include "Capacitor.h"
#include "Inductor.h"
#include "VoltageSource.h"
#include "CurrentSource.h"
#include "SinusoidalVoltageSource.h"
#include "transientdialogbox.h"
#include "acsweepdialog.h"
#include "phasesweepdialog.h"
#include "nodelibrarydialog.h"
#include "circuitescene.h"
#include "graphicalresistor.h"
#include "graphicalcapacitor.h"
#include "graphicalinductor.h"
#include "graphicalground.h"
#include "graphicalvoltagesourcedc.h"
#include "graphicalcurrentsourcedc.h"
#include "graphicalvoltagesourcesin.h"
#include "plotdialog.h"
#include <QToolButton>
#include <QMessageBox>
#include <QMenu>
#include <QDebug>

using namespace std;

static int globalNodeCounter = 0;


MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow) {

    ui->setupUi(this);
    m_circuit = std::make_unique<Circuit>();

    CircuitScene *scene = new CircuitScene(this);
    ui->circuitGraphicsView->setScene(scene);
    ui->circuitGraphicsView->setProperty("currentMode", static_cast<int>(m_currentMode));

    connect(ui->actionAddResistor, &QAction::triggered, this, &MainWindow::on_actionAddResistor_triggered);
    connect(ui->actionAddCapacitor, &QAction::triggered, this, &MainWindow::on_actionAddCapacitor_triggered);
    connect(ui->actionAddInductor, &QAction::triggered, this, &MainWindow::on_actionAddInductor_triggered);
    connect(ui->actionAddGND, &QAction::triggered, this, &MainWindow::on_actionAddGND_triggered);
    connect(ui->actionAddWire, &QAction::triggered, this, &MainWindow::on_actionAddWire_triggered);
    connect(scene, &CircuitScene::sceneClicked, this, &MainWindow::onSceneClicked);
    connect(scene, &CircuitScene::connectionMade, this, &MainWindow::onConnectionMade);

    {
        QMenu *voltageMenu = new QMenu(this);
        voltageMenu->addAction(ui->actionAddVdc);
        voltageMenu->addAction(ui->actionAddVsin);
        QToolButton *voltageButton = new QToolButton(this);
        voltageButton->setText("Voltage Source");
        voltageButton->setMenu(voltageMenu);
        voltageButton->setPopupMode(QToolButton::MenuButtonPopup);
        ui->toolBar->addWidget(voltageButton);
        connect(ui->actionAddVdc, &QAction::triggered, this, &MainWindow::on_actionAddVdc_triggered);
        connect(ui->actionAddVsin, &QAction::triggered, this, &MainWindow::on_actionAddVsin_triggered);
    }
    {
        QMenu *currentMenu = new QMenu(this);
        currentMenu->addAction(ui->actionAddIdc);
        // currentMenu->addAction(ui->actionAddIsin); // You can add this later
        QToolButton *currentButton = new QToolButton(this);
        currentButton->setText("Current Source");
        currentButton->setMenu(currentMenu);
        currentButton->setPopupMode(QToolButton::MenuButtonPopup);
        ui->toolBar->addWidget(currentButton);
        connect(ui->actionAddIdc, &QAction::triggered, this, &MainWindow::on_actionAddIdc_triggered);
    }

    connect(ui->actionTransient, &QAction::triggered, this, &MainWindow::on_actionTransient_triggered);
    connect(ui->actionAcSweep, &QAction::triggered, this, &MainWindow::on_actionAcSweep_triggered);
    connect(ui->actionPhaseSweep, &QAction::triggered, this, &MainWindow::on_actionPhaseSweep_triggered);
}

MainWindow::~MainWindow() {
    delete ui;
}



void MainWindow::on_actionAddResistor_triggered() {
    m_currentMode = PlacementMode::Resistor;
    setCursor(Qt::CrossCursor);
    ui->circuitGraphicsView->setProperty("currentMode", static_cast<int>(m_currentMode));
}

void MainWindow::on_actionAddCapacitor_triggered() {
    m_currentMode = PlacementMode::Capacitor;
    setCursor(Qt::CrossCursor);
    ui->circuitGraphicsView->setProperty("currentMode", static_cast<int>(m_currentMode));
}

void MainWindow::on_actionAddInductor_triggered() {
    m_currentMode = PlacementMode::Inductor;
    setCursor(Qt::CrossCursor);
    ui->circuitGraphicsView->setProperty("currentMode", static_cast<int>(m_currentMode));
}

void MainWindow::on_actionAddGND_triggered() {
    m_currentMode = PlacementMode::Ground;
    setCursor(Qt::CrossCursor);
    ui->circuitGraphicsView->setProperty("currentMode", static_cast<int>(m_currentMode));
}

void MainWindow::on_actionAddVdc_triggered() {
    m_currentMode = PlacementMode::VoltageSource_DC;
    setCursor(Qt::CrossCursor);
    ui->circuitGraphicsView->setProperty("currentMode", static_cast<int>(m_currentMode));
}

void MainWindow::on_actionAddIdc_triggered() {
    m_currentMode = PlacementMode::CurrentSource_DC;
    setCursor(Qt::CrossCursor);
    ui->circuitGraphicsView->setProperty("currentMode", static_cast<int>(m_currentMode));
}

void MainWindow::on_actionAddVsin_triggered() {
    m_currentMode = PlacementMode::VoltageSource_Sin;
    setCursor(Qt::CrossCursor);
    ui->circuitGraphicsView->setProperty("currentMode", static_cast<int>(m_currentMode));
}

void MainWindow::on_actionAddWire_triggered() {
    m_currentMode = PlacementMode::Wire;
    setCursor(Qt::CrossCursor);
    ui->circuitGraphicsView->setProperty("currentMode", static_cast<int>(m_currentMode));
}


void MainWindow::onSceneClicked(const QPointF &location) {

    if (m_currentMode == PlacementMode::Wire) return;
    if (m_currentMode == PlacementMode::Ground) {
        auto graphicalGround = new GraphicalGround();
        ui->circuitGraphicsView->scene()->addItem(graphicalGround);
        graphicalGround->setPos(location);
    }
    else if (m_currentMode > PlacementMode::None && m_currentMode < PlacementMode::Wire) {

        string node1Name = "n" + to_string(globalNodeCounter++);
        string node2Name = "n" + to_string(globalNodeCounter++);

        GraphicalComponent *graphicalComp = nullptr;
        shared_ptr<CircuitElement> logicalComp = nullptr;

        if (m_currentMode == PlacementMode::Resistor) {
            graphicalComp = new GraphicalResistor();
            static int resistorCount = 1;
            string name = "R" + to_string(resistorCount++);
            logicalComp = m_circuit->addResistor(name, node1Name, node2Name, 1);
        } else if (m_currentMode == PlacementMode::Capacitor) {
            graphicalComp = new GraphicalCapacitor();
            static int capCount = 1;
            string name = "C" + to_string(capCount++);
            logicalComp = m_circuit->addCapacitor(name, node1Name, node2Name, 1);
        } else if (m_currentMode == PlacementMode::Inductor) {
            graphicalComp = new GraphicalInductor();
            static int indCount = 1;
            string name = "L" + to_string(indCount++);
            logicalComp = m_circuit->addInductor(name, node1Name, node2Name, 1);
        } else if (m_currentMode == PlacementMode::VoltageSource_DC) {
            graphicalComp = new GraphicalVoltageSource_DC();
            static int vdcCount = 1;
            string name = "V" + to_string(vdcCount++);
            logicalComp = m_circuit->addVoltageSource(name, node1Name, node2Name, 12.0);
        } else if (m_currentMode == PlacementMode::CurrentSource_DC) {
            graphicalComp = new GraphicalCurrentSource_DC();
            static int idcCount = 1;
            string name = "I" + to_string(idcCount++);
            logicalComp = m_circuit->addCurrentSource(name, node1Name, node2Name, 1.0);
        } else if (m_currentMode == PlacementMode::VoltageSource_Sin) {
            graphicalComp = new GraphicalVoltageSource_Sin();
            static int vsinCount = 1;
            string name = "VSin" + to_string(vsinCount++);
            logicalComp = m_circuit->addSinusoidalVoltageSource(name, node1Name, node2Name, 0.0, 5.0, 1000.0, 0.0);

        }

        if (graphicalComp && logicalComp) {
            graphicalComp->setLogicalComponent(logicalComp);
            ui->circuitGraphicsView->scene()->addItem(graphicalComp);
            graphicalComp->setPos(location);
            graphicalComp->updateNodeLabels();
        }

    }


    m_currentMode = PlacementMode::None;
    setCursor(Qt::ArrowCursor);
    ui->circuitGraphicsView->setProperty("currentMode", static_cast<int>(m_currentMode));
}

void MainWindow::onConnectionMade(GraphicalComponent *startComp, int startTerminalIndex, GraphicalComponent *endComp, int endTerminalIndex)
{

    auto groundComp = dynamic_cast<GraphicalGround*>(startComp);
    auto otherComp = endComp;
    int otherTerminalIndex = endTerminalIndex;

    if (!groundComp) {
        groundComp = dynamic_cast<GraphicalGround*>(endComp);
        otherComp = startComp;
        otherTerminalIndex = startTerminalIndex;
    }

    if (groundComp) {


        auto logicalToGround = otherComp->getLogicalComponent();
        if (logicalToGround) {

            std::string nodeToGroundName = (otherTerminalIndex == 0) ?
                                          logicalToGround->getNode1()->getName() :
                                          logicalToGround->getNode2()->getName();


            m_circuit->setNodeAsGround(nodeToGroundName);

            qDebug() << "Node" << QString::fromStdString(nodeToGroundName) << "is now connected to GROUND.";
        }
    } else {
        auto startLogical = startComp->getLogicalComponent();
        auto endLogical = endComp->getLogicalComponent();

        if (startLogical && endLogical) {
            std::string nodeToKeep = (startTerminalIndex == 0) ? startLogical->getNode1()->getName() : startLogical->getNode2()->getName();
            std::string nodeToRemove = (endTerminalIndex == 0) ? endLogical->getNode1()->getName() : endLogical->getNode2()->getName();

            m_circuit->mergeNodes(nodeToKeep, nodeToRemove);
            qDebug() << "Merged node" << QString::fromStdString(nodeToRemove) << "into" << QString::fromStdString(nodeToKeep);
        }
    }

    for (QGraphicsItem *item : ui->circuitGraphicsView->scene()->items()) {
        if (auto comp = dynamic_cast<GraphicalComponent*>(item)) {
            comp->updateNodeLabels();
        }
    }

    m_currentMode = PlacementMode::None;
    setCursor(Qt::ArrowCursor);
    ui->circuitGraphicsView->setProperty("currentMode", static_cast<int>(m_currentMode));
}




void MainWindow::on_actionTransient_triggered()
{
    // یک نمونه از دیالوگ تحلیل بساز
    transientDialogBox dialog(this);

    // دیالوگ را نمایش بده و منتظر بمان تا کاربر OK یا Cancel را بزند
    if (dialog.exec() == QDialog::Accepted) {

        // اگر کاربر OK را زد، تمام پارامترها را از دیالوگ بخوان
        std::string analysisType = dialog.getAnalysisType();
        double startTime = dialog.getStartTime();
        double timeStep = dialog.getTimeStep();
        double stopTime = dialog.getStopTime();
        std::string outputName = dialog.getOutputName();

        // متغیرهایی برای نگهداری نتایج و اطلاعات نمودار
        std::vector<std::pair<double, double>> results;
        QString plotTitle;
        QString yAxisLabel;
        QString xAxisLabel = "Time (s)";

        // بر اساس انتخاب کاربر، تابع تحلیل مناسب را از بک‌اند صدا بزن
        if (analysisType == "Voltage") {
            results = m_circuit->runVoltageTransientAnalysis(startTime, timeStep, stopTime, outputName);
            plotTitle = "Voltage Transient Analysis for " + QString::fromStdString(outputName);
            yAxisLabel = "Voltage (V)";
        } else if (analysisType == "Current") {
            results = m_circuit->runCurrentTransientAnalysis(startTime, timeStep, stopTime, outputName);
            plotTitle = "Current Transient Analysis for " + QString::fromStdString(outputName);
            yAxisLabel = "Current (A)";
        }

        // بررسی کن که آیا تحلیلی انجام شده و نتیجه‌ای برگشته است یا نه
        if (!results.empty()) {
            // اگر نتیجه وجود داشت، پنجره نمودار را با داده‌ها و عناوین مناسب بساز و نمایش بده
            PlotDialog plotDialog(results, plotTitle, xAxisLabel, yAxisLabel, this);
            plotDialog.exec();
        } else {
            // اگر نتیجه‌ای وجود نداشت، یک پیغام خطای مناسب به کاربر نشان بده
            QMessageBox::warning(this, "Analysis Error",
                                 "Analysis failed or produced no results. "
                                 "Please check your circuit connections and analysis parameters.");
        }
    }
}

void MainWindow::on_actionAcSweep_triggered()
{
    AcSweepDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {

        double startOmega = dialog.getStartFrequency();
        double endOmega = dialog.getEndFrequency();
        int steps = dialog.getNumberOfSteps();
        std::string outputName = dialog.getOutputName();

        auto results = m_circuit->runACAnalysis(startOmega, endOmega, steps, outputName);


        if (!results.empty()) {
            QString plotTitle = "AC Sweep Analysis for " + QString::fromStdString(outputName);
            QString xAxisLabel = "Angular Frequency (rad/s)";
            QString yAxisLabel = "Magnitude";

            PlotDialog plotDialog(results, plotTitle, xAxisLabel, yAxisLabel, this);
            plotDialog.exec();
        } else {
            QMessageBox::warning(this, "Analysis Error",
                                 "AC analysis failed. Please ensure there is at least one sinusoidal source and check parameters.");
        }
    }
}

void MainWindow::on_actionPhaseSweep_triggered()
{
    PhaseSweepDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {

        double startPhase = dialog.getStartPhase();
        double endPhase = dialog.getEndPhase();
        int steps = dialog.getNumberOfSteps();
        std::string outputName = dialog.getOutputName();


        auto results = m_circuit->runPhaseSweepAnalysis(startPhase, endPhase, steps, outputName);

        if (!results.empty()) {
            QString plotTitle = "Phase Sweep Analysis for " + QString::fromStdString(outputName);
            QString xAxisLabel = "Phase (Degrees)";
            QString yAxisLabel = "Magnitude";

            PlotDialog plotDialog(results, plotTitle, xAxisLabel, yAxisLabel, this);
            plotDialog.exec();
        } else {
            QMessageBox::warning(this, "Analysis Error",
                                 "Phase Sweep analysis failed. Please ensure there is exactly one sinusoidal source in the circuit.");
        }
    }
}

void MainWindow::on_actionOpenNodeLibrary_triggered() {
    NodeLibraryDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        string selected = dialog.getSelectedComponent();
        qDebug() << "User selected:" << QString::fromStdString(selected);
    }
}