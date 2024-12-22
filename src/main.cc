#include <iostream>
#include "../include/time.h"

int main() {
    try {
        Time time = Time::fromString("12:30");
        std::cout << time.getHour() << "  " << time.getMinute() << std::endl;
        std::cout << time << std::endl;
    } catch(const std::invalid_argument &e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}