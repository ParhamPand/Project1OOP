cmake_minimum_required(VERSION 3.29)
project(project1 CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 COMPONENTS Widgets PrintSupport REQUIRED)


set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/frontend/Graphics)

add_executable(project1
        main.cpp

        # --- Frontend Source & Header Files ---
        frontend/src/mainwindow.cpp
        frontend/include/mainwindow.h
        frontend/src/transientdialogbox.cpp
        frontend/include/transientdialogbox.h
        frontend/src/acsweepdialog.cpp
        frontend/include/acsweepdialog.h
        frontend/src/phasesweepdialog.cpp
        frontend/include/phasesweepdialog.h

        # --- Frontend UI Files ---
        frontend/Graphics/mainwindow.ui
        frontend/Graphics/transientdialogbox.ui
        frontend/Graphics/acsweepdialog.ui
        frontend/Graphics/phasesweepdialog.ui
        frontend/Graphics/propertiesdialog.ui
        frontend/Graphics/sinusoidalsourcedialog.ui

        # --- Backend Source Files ---
        backend/src/Node.cpp
        backend/src/CircuitElement.cpp
        backend/src/Resistor.cpp
        backend/src/Capacitor.cpp
        backend/src/Inductor.cpp
        backend/src/Circuit.cpp
        backend/src/VoltageSource.cpp
        backend/src/CurrentSource.cpp
        backend/src/IdealDiode.cpp
        backend/src/ZenerDiode.cpp
        backend/src/SinusoidalVoltageSource.cpp
        backend/src/MatrixSolver.cpp
        frontend/include/nodelibrarydialog.h
        frontend/src/nodelibrarydialog.cpp
        frontend/src/graphicalcomponent.cpp
        frontend/include/graphicalcomponent.h
        frontend/include/graphicalresistor.h
        frontend/src/graphicalresistor.cpp
        frontend/include/circuitescene.h
        frontend/src/circuitescene.cpp
        frontend/include/propertiesdialog.h
        frontend/src/propertiesdialog.cpp
        frontend/include/graphicalcapacitor.h
        frontend/src/graphicalcapacitor.cpp
        frontend/include/graphicalinductor.h
        frontend/src/graphicalinductor.cpp
        frontend/include/graphicalground.h
        frontend/src/graphicalground.cpp
        frontend/include/graphicalvoltagesourcedc.h
        frontend/src/graphicalvoltagesourcedc.cpp
        frontend/include/graphicalcurrentsourcedc.h
        frontend/src/graphicalcurrentsourcedc.cpp
        frontend/include/graphicalvoltagesourcesin.h
        frontend/src/graphicalvoltagesourcesin.cpp
        frontend/include/sinusoidalsourcedialog.h
        frontend/src/sinusoidalsourcedialog.cpp
        frontend/include/graphicalwire.h
        frontend/src/graphicalwire.cpp
        frontend/include/nodelabelitem.h
        frontend/src/nodelabelitem.cpp
        frontend/include/qcustomplot.h
        frontend/src/qcustomplot.cpp
        frontend/include/plotdialog.h
        frontend/src/plotdialog.cpp
        frontend/Graphics/plotdialog.ui
        backend/include/ComplexMatrixSolver.h
        backend/src/ComplexMatrixSolver.cpp

)

# Add ALL header locations to the include path
target_include_directories(project1 PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/backend/include
        ${CMAKE_CURRENT_SOURCE_DIR}/frontend/include
        ${CMAKE_CURRENT_SOURCE_DIR}/cereal/include
)

target_link_libraries(project1 PRIVATE Qt6::Widgets Qt6::PrintSupport)