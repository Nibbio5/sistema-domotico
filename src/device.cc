// Eros Menin

#include "../include/device.h"

Device::Device(const std::string& name, const double power, const Time& startTime) : kid_{SingletonId::getInstance().generateId()}, kname_(name), kpower_(power), is_on{false}, start_time_{std::make_shared<Time>(startTime)} {}

Device::Device(const std::string& name, const double power) : kid_{SingletonId::getInstance().generateId()}, kname_(name), kpower_(power), is_on{false}, start_time_{nullptr} {}

std::shared_ptr<const Time> Device::getStartTime() const {return start_time_;}

bool Device::isOn() const {return is_on;}

void Device::switchOn() {is_on = true;}

void Device::switchOff() {is_on = false;}

std::ostream& operator<<(std::ostream& out, const Device& device) {
    out << "Device{"
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

    out << "}";

    return out;
}