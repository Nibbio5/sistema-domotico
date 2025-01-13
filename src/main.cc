#include "../include/terminal.h"
#include <iostream>
#include <string>

/**
 * @brief Prints a welcome string and starts the terminal using the given power
 *
 * @param power the power limit of the domotic system
 */
void invokeTerminal(double power = 0.5) {
    std::cout << "\n\t\tBenvenuto a casa ;)\n" << std::endl;
    Terminal terminal{power};
    terminal.run();
}

/**
 * @brief Reads the first given parameters from the command line and uses it as the
 * domotic system power. Than starts the system terminal.
 *
 * The given parameter must be a decimal or non decimal number.
 *
 * @param argc the amount of given parameters
 * @param argv the array containing the given parameters
 * @return int: the state of the program, either 0 (terminated successfully) or 1
 * (terminated with errors)
 */
int main(int argc, char* argv[]) {
    if(argc < 2) {
        std::cout << "Nessuna potenza fornita, il programma utilizzerà il valore di default 3.5kW\n\n";
        invokeTerminal();
    } else {
        double power;

        try {
            power = std::stod(argv[1]);
        } catch(const std::exception& e) {
            std::cout << "Errore: Il valore inserito come potenza risulta invalido" << std::endl;
            return 1;
        }

        invokeTerminal(power);
    }

    return 0;
}