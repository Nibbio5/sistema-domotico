#include "..\include\device.h"
#include <string>
#include <iostream>

int main() {
    Device d {"cacca", 12.3};
    std::cout << d.id << std::endl;
    std::cout << d.name << std::endl;
    std::cout << d.power << std::endl;
    std::cout << d.isDeviceOn() << std::endl << std::endl;
    
    Device d1 {"merda", 1.2};
    std::cout << d1.id << std::endl;
    std::cout << d1.name << std::endl;
    std::cout << d1.power << std::endl;
    std::cout << d1.isDeviceOn() << std::endl << std::endl;

    return 0;
}
