#include "../include/manual_device.h"
#include "../include/cp_device.h"
#include "../include/device.h"
#include "../include/time.h"
#include <string>
#include <iostream>
#include <chrono>

int aaa(Device a) {return 1;}

int main() {

    ManualDevice md {"cacca", 12.3, Time().fromString("12:44"), Time().fromString("12:44")};
    std::cout << md << "\n\n";
    
    CPDevice cpd {"merda", 1.2, std::chrono::duration_cast<std::chrono::seconds>(md.getStartTime() - md.getStopTime())};
    std::cout << cpd << "\n\n";

    try {
        Time time = Time::fromString("12:30");
        std::cout << time.getHour() << "  " << time.getMinute() << std::endl;
        std::cout << time << std::endl;
    } catch(const std::invalid_argument &e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}