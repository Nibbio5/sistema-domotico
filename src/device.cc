// Eros Menin

#include "../include/device.h"

Device::Device(const std::string& name, const double power, const Time& start_time) : KId{SingletonId::getInstance().generateId()}, KName(name), KPower(power), start_time_{std::make_shared<Time>(start_time)} {}

Device::Device(const std::string& name, const double power) : KId{SingletonId::getInstance().generateId()}, KName(name), KPower(power), start_time_{nullptr} {}

std::shared_ptr<const Time> Device::get_start_time() const {return start_time_;}

bool Device::is_on() const {return is_on_;}

void Device::switch_on() {is_on_ = true;}

void Device::switch_off() {is_on_ = false;}

std::ostream& operator<<(std::ostream& out, const Device& device) {
    out << "Device{"
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

    out << "}";

    return out;
}
