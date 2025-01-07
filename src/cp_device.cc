// Eros Menin

#include "../include/cp_device.h"

CPDevice::CPDevice(const std::string& name, const double power, const Time& duration, const Time& start_time) : Device(name, power, start_time), KDuration{duration} {}

CPDevice::CPDevice(const std::string& name, const double power, const Time& duration) : Device(name, power), KDuration{duration} {}

void CPDevice::setStartTime(const Time& newTime) {
    std::shared_ptr<Time> newStartTime_ptr = std::make_shared<Time>(newTime);
    startTime = newStartTime_ptr;
    }

void CPDevice::reset_start_time() {start_time_.reset();}

std::ostream& operator<<(std::ostream& out, const CPDevice& device) {
    out << "CPDevice{"
        << "id=" << device.KId
        << ", name=" << device.KName
        << ", power=" << device.KPower
        << ", is_on=" << device.is_on_
        << ", start_time=";

    if(device.start_time_){
        out << *device.start_time_;
    }else{
        out << "NOT_SET";
    }

    out << ", duration=" << device.KDuration
        << "}";

    return out;
}
