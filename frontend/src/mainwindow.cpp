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
#include <QToolButton>
#include <QMenu>
#include <QDebug>

using namespace std;

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

// --- اسلات‌ها برای تغییر حالت برنامه (با setProperty) ---

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


// --- اسلات برای قرار دادن قطعه روی صفحه ---

void MainWindow::onSceneClicked(const QPointF &location) {
    if (m_currentMode == PlacementMode::Resistor) {
        auto graphicalResistor = new GraphicalResistor();
        static int resistorCount = 1;
        string name = "R" + to_string(resistorCount++);
        auto logicalResistor = m_circuit->addResistor(name, "0", "1", 1000.0);
        graphicalResistor->setLogicalComponent(std::static_pointer_cast<CircuitElement>(logicalResistor));
        ui->circuitGraphicsView->scene()->addItem(graphicalResistor);
        graphicalResistor->setPos(location);
    }
    else if (m_currentMode == PlacementMode::Capacitor) {
        auto graphicalCapacitor = new GraphicalCapacitor();
        static int capCount = 1;
        string name = "C" + to_string(capCount++);
        auto logicalCapacitor = m_circuit->addCapacitor(name, "0", "1", 1e-6);
        graphicalCapacitor->setLogicalComponent(std::static_pointer_cast<CircuitElement>(logicalCapacitor)); // اصلاح شده
        ui->circuitGraphicsView->scene()->addItem(graphicalCapacitor);
        graphicalCapacitor->setPos(location);
    }
    else if (m_currentMode == PlacementMode::Inductor) {
        auto graphicalInductor = new GraphicalInductor();
        static int indCount = 1;
        string name = "L" + to_string(indCount++);
        auto logicalInductor = m_circuit->addInductor(name, "0", "1", 1e-3);
        graphicalInductor->setLogicalComponent(std::static_pointer_cast<CircuitElement>(logicalInductor)); // اصلاح شده
        ui->circuitGraphicsView->scene()->addItem(graphicalInductor);
        graphicalInductor->setPos(location);
    }
    else if (m_currentMode == PlacementMode::Ground) {
        auto graphicalGround = new GraphicalGround();
        ui->circuitGraphicsView->scene()->addItem(graphicalGround);
        graphicalGround->setPos(location);
    }
    else if (m_currentMode == PlacementMode::VoltageSource_DC) {
        auto graphicalVdc = new GraphicalVoltageSource_DC();
        static int vdcCount = 1;
        string name = "V" + to_string(vdcCount++);
        auto logicalVdc = m_circuit->addVoltageSource(name, "0", "1", 12.0);
        graphicalVdc->setLogicalComponent(std::static_pointer_cast<CircuitElement>(logicalVdc));
        ui->circuitGraphicsView->scene()->addItem(graphicalVdc);
        graphicalVdc->setPos(location);
    }
    else if (m_currentMode == PlacementMode::CurrentSource_DC) {
        auto graphicalIdc = new GraphicalCurrentSource_DC();
        static int idcCount = 1;
        string name = "I" + to_string(idcCount++);
        auto logicalIdc = m_circuit->addCurrentSource(name, "0", "1", 1.0);
        graphicalIdc->setLogicalComponent(std::static_pointer_cast<CircuitElement>(logicalIdc));
        ui->circuitGraphicsView->scene()->addItem(graphicalIdc);
        graphicalIdc->setPos(location);
    }
    else if (m_currentMode == PlacementMode::VoltageSource_Sin) {
        auto graphicalVsin = new GraphicalVoltageSource_Sin();
        static int vsinCount = 1;
        string name = "VSin" + to_string(vsinCount++);
        auto logicalVsin = m_circuit->addSinusoidalVoltageSource(name, "0", "1", 0.0, 5.0, 1000.0, 0.0);
        graphicalVsin->setLogicalComponent(std::static_pointer_cast<CircuitElement>(logicalVsin));
        ui->circuitGraphicsView->scene()->addItem(graphicalVsin);
        graphicalVsin->setPos(location);
    }

    // بازنشانی حالت برنامه و آپدیت تابلوی اعلانات
    m_currentMode = PlacementMode::None;
    setCursor(Qt::ArrowCursor);
    ui->circuitGraphicsView->setProperty("currentMode", static_cast<int>(m_currentMode));
}

void MainWindow::onConnectionMade(GraphicalComponent *startComp, int startTerminalIndex, GraphicalComponent *endComp, int endTerminalIndex)
{
    auto startLogical = startComp->getLogicalComponent();
    auto endLogical = endComp->getLogicalComponent();

    if (startLogical && endLogical) {
        // نام گره‌های منطقی را بگیرید
        std::string nodeToKeep = (startTerminalIndex == 0) ? startLogical->getNode1()->getName() : startLogical->getNode2()->getName();
        std::string nodeToRemove = (endTerminalIndex == 0) ? endLogical->getNode1()->getName() : endLogical->getNode2()->getName();

        // به بک‌اند دستور ادغام بدهید
        m_circuit->mergeNodes(nodeToKeep, nodeToRemove);

        // (اختیاری) یک پیغام برای تست چاپ کنید
        qDebug() << "Merged node" << QString::fromStdString(nodeToRemove) << "into" << QString::fromStdString(nodeToKeep);
    }

    // برنامه را به حالت عادی برگردانید
    m_currentMode = PlacementMode::None;
    setCursor(Qt::ArrowCursor);
    ui->circuitGraphicsView->setProperty("currentMode", static_cast<int>(m_currentMode));
}




void MainWindow::on_actionTransient_triggered() {
    transientDialogBox dialog(this);
    dialog.exec();
}

void MainWindow::on_actionAcSweep_triggered() {
    AcSweepDialog dialog(this);
    dialog.exec();
}

void MainWindow::on_actionPhaseSweep_triggered() {
    PhaseSweepDialog dialog(this);
    dialog.exec();
}

void MainWindow::on_actionOpenNodeLibrary_triggered() {
    NodeLibraryDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        string selected = dialog.getSelectedComponent();
        qDebug() << "User selected:" << QString::fromStdString(selected);
    }
}