// Eros Menin

#include "../include/cp_device.h"

CPDevice::CPDevice(const std::string& name, const double power, const Time& duration, const Time& startTime) : Device(name, power, startTime), kduration_{duration} {}

CPDevice::CPDevice(const std::string& name, const double power, const Time& duration) : Device(name, power), kduration_{duration} {}

void CPDevice::setStartTime(const Time& newTime) {*start_time_ = newTime;}

void CPDevice::resetStartTime() {start_time_.reset();}

std::ostream& operator<<(std::ostream& out, const CPDevice device) {
    out << "CPDevice{"
        << "id=" << device.kid_
        << ", name=" << device.kname_
        << ", power=" << device.kpower_
        << ", isOn=" << device.isOn()
        << ", startTime=";

    if(device.getStartTime()){
        out << *device.getStartTime();
    }else{
        out << "NOT_SET";
    }

    out << ", duration=" << device.kduration_
        << "}";

    return out;
}