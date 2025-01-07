// #include "../include/domotics_system.h"
#include "../include/manual_device.h"
#include "../include/cp_device.h"
#include <string>
#include <iostream>
#include <memory>

int main() {

    ManualDevice md {"cacca", 60, Time(12, 44), Time(13, 11)};
    std::cout << md << "\n\n";
    CPDevice cpd {"cacca", -2, Time(22, 22)};
    std::cout << cpd << "\n\n";
    
    md.switch_on(Time(12, 33));
    // md.switch_off(Time(12, 32));
    std::cout << md.get_total_power(Time(12, 34)) << "\n\n";

    md.switch_on(Time(12, 35));
    // md.switch_off(Time(12, 32));
    std::cout << md.get_total_power(Time(12, 35)) << "\n\n";

    md.switch_off(Time(12, 37));
    // md.switch_off(Time(12, 32));
    std::cout << md.get_total_power(Time(12, 37)) << "\n\n";
    std::cout << md.get_total_power(Time(12, 38)) << "\n\n";

    md.resetDeviceAndTimer();
    std::cout << md << "\n\n";

    cpd.resetDeviceAndTimer();
    std::cout << cpd << "\n\n";

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


