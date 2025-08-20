#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Circuit.h"
#include <memory>

enum class PlacementMode {
    None,
    Resistor,
    Capacitor,
    Inductor,
    Ground,
    VoltageSource_DC,
    CurrentSource_DC,
    VoltageSource_Sin

};

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void on_actionTransient_triggered();
    void on_actionAcSweep_triggered();
    void on_actionPhaseSweep_triggered();
    void on_actionOpenNodeLibrary_triggered();
    void on_actionAddResistor_triggered();
    void on_actionAddCapacitor_triggered();
    void on_actionAddInductor_triggered();
    void on_actionAddGND_triggered();
    void on_actionAddVdc_triggered();
    void on_actionAddIdc_triggered();
    void on_actionAddVsin_triggered();
    void onSceneClicked(const QPointF &location);

private:
    Ui::MainWindow *ui;
    std::unique_ptr<Circuit> m_circuit;
    PlacementMode m_currentMode = PlacementMode::None;

};

#endif // MAINWINDOW_H