#include "../include/domotics_system.h";

int main() {
    DomoticsSystem sistema;
    //Time t1(8,0);
    //sistema.setCurrentTime(t1);
    //sistema.changeDeviceStatus(true,"Pompa di calore");
    //sistema.changeDeviceStatus(false,"Pompa di calore");
    sistema.setCurrentTime(Time (0,1));
    sistema.setCurrentTime(Time(0,2));
    //sistema.changeDeviceStatus(true,"Lavastoviglie");
    //sistema.setCurrentTime(Time(10,04));
    //std::cout << sistema << std::endl;
    //sistema.setCurrentTime(Time(9,35));
    //sistema.changeDeviceStatus(true,"Lavatrice");
    //sistema.changeDeviceStatus(true,"Lavastoviglie");
    sistema.setDeviceTime("Lavatrice",Time(8,15),Time(8,30));
    sistema.setCurrentTime(Time(8,00));
    sistema.setCurrentTime(Time(8,20));
    sistema.changeDeviceStatus(false,"Lavatrice");
    sistema.removeDeviceTimer("Lavatrice");
    sistema.setCurrentTime(Time(15,31));
    sistema.resetTime();
    return 0;
}