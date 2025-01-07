// #include "../include/domotics_system.h"
#include "../include/manual_device.h"
#include <string>
#include <iostream>


int main() {

    ManualDevice md {"cacca", 12.3, Time().fromString("12:44"), Time().fromString("12:44")};
    std::cout << md << "\n\n";

    // try {
    //     Time time = Time::fromString("12:30");
    //     std::cout << time.getHour() << "  " << time.getMinute() << std::endl;
    //     std::cout << time << std::endl;
    // } catch(const std::invalid_argument &e) {
    //     std::cerr << e.what() << std::endl;
    // }
    // auto t1 =  Time(8, 10);
    // auto sistema = DomoticsSystem();
    // std::cout << sistema << std::endl;
    // sistema.setCurrentTime(t1);
    // sistema.currentMod();
    // sistema.changeDeviceStatus(true, "Pompa di calore + termostato");
    //std::cout << sistema << std::endl;
    return 0;
}


