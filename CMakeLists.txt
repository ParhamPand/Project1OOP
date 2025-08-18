cmake_minimum_required(VERSION 3.29)
project(project1 CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

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
        backend/src/FileManager.cpp
        frontend/include/nodelibrarydialog.h
        frontend/src/nodelibrarydialog.cpp
)

# Add ALL header locations to the include path
target_include_directories(project1 PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/backend/include
        ${CMAKE_CURRENT_SOURCE_DIR}/frontend/include
)

# Link the Qt libraries
target_link_libraries(project1 PRIVATE Qt6::Widgets)