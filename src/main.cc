#include "../include/domotics_system.h"
#include <string>
#include <iostream>


int main() {
    Time t1=  Time (8,10);
    DomoticsSystem sistema = DomoticsSystem();
    std::cout << sistema << std::endl;
    sistema.setCurrentTime(t1);
    sistema.changeDeviceStatus(true,"Pompa di calore");
        //sistema.changeDeviceStatus(false,"Pompa di calore");
    //sistema.changeDeviceStatus(true,"Lavatrice");
    //sistema.changeDeviceStatus(true,"Lavastoviglie");
    sistema.setDeviceTime("Lavatrice",Time(8,15),Time(8,30));
    sistema.setCurrentTime(Time(8,20));
    sistema.setCurrentTime(Time(15,31));
    return 0;
}