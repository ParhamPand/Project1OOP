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
)

target_include_directories(project1 PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
