// Eros Menin

#include "../include/cp_device.h"

CPDevice::CPDevice(const std::string& name, const double power, const Time& duration, const Time& startTime) : Device(name, power, startTime), duration{duration} {}

CPDevice::CPDevice(const std::string& name, const double power, const Time& duration) : Device(name, power), duration{duration} {}

void CPDevice::setStartTime(const Time& newTime) {*startTime = newTime;}

void CPDevice::resetStartTime() {startTime.reset();}

std::ostream& operator<<(std::ostream& out, const CPDevice& device) {
    out << "CPDevice{"
        << "id=" << device.id
        << ", name=" << device.name
        << ", power=" << device.power
        << ", isOn=" << device.isDeviceOn()
        << ", startTime=";

    if(device.getStartTime()){
        out << *device.getStartTime();
    }else{
        out << "NOT_SET";
    }

    out << ", duration=" << device.duration
        << "}";

    return out;
}