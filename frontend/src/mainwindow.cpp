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



MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow) {

    ui->setupUi(this);
    m_circuit = std::make_unique<Circuit>();

    CircuitScene *scene = new CircuitScene(this);
    ui->circuitGraphicsView->setScene(scene);

    connect(ui->actionAddResistor, &QAction::triggered, this, &MainWindow::on_actionAddResistor_triggered);
    connect(ui->actionAddCapacitor, &QAction::triggered, this, &MainWindow::on_actionAddCapacitor_triggered);
    connect(ui->actionAddInductor, &QAction::triggered, this, &MainWindow::on_actionAddInductor_triggered);
    connect(ui->actionAddGND, &QAction::triggered, this, &MainWindow::on_actionAddGND_triggered);
    connect(scene, &CircuitScene::sceneClicked, this, &MainWindow::onSceneClicked);

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
        currentMenu->addAction(ui->actionAddIsin);
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

void MainWindow::on_actionTransient_triggered()
{
    transientDialogBox dialog(this);
    dialog.exec();
}

void MainWindow::on_actionAcSweep_triggered()
{
    AcSweepDialog dialog(this);
    dialog.exec();
}

void MainWindow::on_actionPhaseSweep_triggered()
{
    PhaseSweepDialog dialog(this);
    dialog.exec();
}

void MainWindow::on_actionOpenNodeLibrary_triggered()
{
    NodeLibraryDialog dialog(this);

    // 1. Check if the user made a choice
    if (dialog.exec() == QDialog::Accepted)
    {
        // 2. Get the specific choice from the dialog
        std::string selected = dialog.getSelectedComponent();
        qDebug() << "User selected:" << QString::fromStdString(selected);
    }
}

void MainWindow::on_actionAddResistor_triggered()
{
    m_currentMode = PlacementMode::Resistor;
    setCursor(Qt::CrossCursor);

}

void MainWindow::on_actionAddCapacitor_triggered()
{
    m_currentMode = PlacementMode::Capacitor;
    setCursor(Qt::CrossCursor);
}

void MainWindow::on_actionAddInductor_triggered()
{
    m_currentMode = PlacementMode::Inductor;
    setCursor(Qt::CrossCursor);
}

void MainWindow::on_actionAddGND_triggered()
{
    // For now, we can just set the placement mode.
    // We can add specific logic for the ground component later if needed.
    m_currentMode = PlacementMode::Ground;
    setCursor(Qt::CrossCursor);
}

void MainWindow::on_actionAddVdc_triggered()
{
    m_currentMode = PlacementMode::VoltageSource_DC; // We need to add this to our enum
    setCursor(Qt::CrossCursor);
}

void MainWindow::on_actionAddIdc_triggered()
{
    m_currentMode = PlacementMode::CurrentSource_DC;
    setCursor(Qt::CrossCursor);
}

void MainWindow::on_actionAddVsin_triggered()
{
    m_currentMode = PlacementMode::VoltageSource_Sin;
    setCursor(Qt::CrossCursor);
}

void MainWindow::onSceneClicked(const QPointF &location)
{

    if (m_currentMode == PlacementMode::Resistor) {
        GraphicalResistor *graphicalResistor = new GraphicalResistor();
        static int resistorCount = 1;
        std::string resistorName = "R" + std::to_string(resistorCount++);
        auto logicalResistor = m_circuit->addResistor(resistorName, "0", "1", 1.0);
        graphicalResistor->setLogicalComponent(std::static_pointer_cast<CircuitElement>(logicalResistor));
        ui->circuitGraphicsView->scene()->addItem(graphicalResistor);
        graphicalResistor->setPos(location);

    }
    else if (m_currentMode == PlacementMode::Capacitor) {
        auto graphicalCapacitor = new GraphicalCapacitor();

        static int capCount = 1;
        std::string name = "C" + std::to_string(capCount++);
        auto logicalCapacitor = m_circuit->addCapacitor(name, "0", "1", 1.0);

        graphicalCapacitor->setLogicalComponent(logicalCapacitor);
        ui->circuitGraphicsView->scene()->addItem(graphicalCapacitor);
        graphicalCapacitor->setPos(location);
    }
    else if (m_currentMode == PlacementMode::Inductor) {
        auto graphicalInductor = new GraphicalInductor();

        static int indCount = 1;
        std::string name = "L" + std::to_string(indCount++);
        auto logicalInductor = m_circuit->addInductor(name, "0", "1", 1.0);

        graphicalInductor->setLogicalComponent(logicalInductor);
        ui->circuitGraphicsView->scene()->addItem(graphicalInductor);
        graphicalInductor->setPos(location);
    }
    else if (m_currentMode == PlacementMode::Ground) {
        auto graphicalGround = new GraphicalGround();

        // زمین در بک‌اند معمولاً فقط یک گره خاص است و یک قطعه جدا نیست
        // ما بعداً منطق اتصال به گره زمین رو پیاده‌سازی می‌کنیم
        // فعلاً نیازی به ساخت آبجکت منطقی جدا برای اون نیست

        ui->circuitGraphicsView->scene()->addItem(graphicalGround);
        graphicalGround->setPos(location);
    }
    else if (m_currentMode == PlacementMode::VoltageSource_DC) {
        auto graphicalVdc = new GraphicalVoltageSource_DC();

        static int vdcCount = 1;
        std::string name = "V" + std::to_string(vdcCount++);
        auto logicalVdc = m_circuit->addVoltageSource(name, "0", "1", 1);

        graphicalVdc->setLogicalComponent(std::static_pointer_cast<CircuitElement>(logicalVdc));
        ui->circuitGraphicsView->scene()->addItem(graphicalVdc);
        graphicalVdc->setPos(location);
    }
    else if (m_currentMode == PlacementMode::CurrentSource_DC) {
        auto graphicalIdc = new GraphicalCurrentSource_DC();

        static int idcCount = 1;
        std::string name = "I" + std::to_string(idcCount++);
        auto logicalIdc = m_circuit->addCurrentSource(name, "0", "1", 1.0);

        graphicalIdc->setLogicalComponent(std::static_pointer_cast<CircuitElement>(logicalIdc));
        ui->circuitGraphicsView->scene()->addItem(graphicalIdc);
        graphicalIdc->setPos(location);
    }
    else if (m_currentMode == PlacementMode::VoltageSource_Sin) {
        auto graphicalVsin = new GraphicalVoltageSource_Sin();

        static int vsinCount = 1;
        std::string name = "VSin" + std::to_string(vsinCount++);
        auto logicalVsin = m_circuit->addSinusoidalVoltageSource(name, "0", "1", 0.0, 1, 1 , 0);

        graphicalVsin->setLogicalComponent(std::static_pointer_cast<CircuitElement>(logicalVsin));
        ui->circuitGraphicsView->scene()->addItem(graphicalVsin);
        graphicalVsin->setPos(location);
    }

    m_currentMode = PlacementMode::None;
    setCursor(Qt::ArrowCursor);
}