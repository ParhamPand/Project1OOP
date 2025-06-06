#include "Circuit.h"
#include <iostream>
#include <string>

using namespace std;

int main() {
    Circuit myCircuit;
    std::string command;

    while (true) {
        getline(std::cin, command);

        if (command == "exit") {
            break;
        }
        if (command == "print") {
            myCircuit.printCircuitDetails();
            continue;
        }

        myCircuit.handleCommand(command);
    }
    return 0;
}