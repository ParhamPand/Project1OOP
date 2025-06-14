cmake_minimum_required(VERSION 3.29)
project(project1 CXX)

set(CMAKE_CXX_STANDARD 20)

add_executable(project1 main.cpp
        src/Node.cpp
        include/Node.h
        include/CircuitElement.h
        src/CircuitElement.cpp
        include/Resistor.h
        src/Resistor.cpp
        include/Capacitor.h
        src/Capacitor.cpp
        include/Inductor.h
        src/Inductor.cpp
        include/Circuit.h
        src/Circuit.cpp
        include/VoltageSource.h
        src/VoltageSource.cpp
        src/CurrentSource.cpp
        include/CurrentSource.h
        include/IdealDiode.h
        src/IdealDiode.cpp
        include/ZenerDiode.h
        src/ZenerDiode.cpp
        include/SinusoidalVoltageSource.h
        src/SinusoidalVoltageSource.cpp
        include/MatrixSolver.h
        src/MatrixSolver.cpp
        include/FileManager.h
        src/FileManager.cpp
)

target_include_directories(project1 PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
