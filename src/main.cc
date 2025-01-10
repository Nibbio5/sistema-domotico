#include "../include/terminal.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {

    if(argc < 2){
        std::cout << "Nessuna potenza fornita, il programma utilizzerà il valore di default 3.5kW\n\n";
    }else{
        double power;
        
        try{
            power = std::stod(argv[1]);
        }catch(const std::exception& e){
            std::cout << "Errore: Il valore inserito come potenza risulta invalido" << std::endl;
            return 1;
        }
        
        // chiamata a terminal.run() a cui viene passata la potenza
    }

    Terminal terminal;
    terminal.run();
    return 0;
}
