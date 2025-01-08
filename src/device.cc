// Eros Menin

#include "../include/device.h"

Device::Device(const std::string& name, const double power, const Time& start_time) : KId{SingletonId::getInstance().generateId()}, KName(name), KPower(power), start_time_{std::make_shared<Time>(start_time)} {}

Device::Device(const std::string& name, const double power) : KId{SingletonId::getInstance().generateId()}, KName(name), KPower(power) {}

std::shared_ptr<const Time> Device::get_start_time() const {return start_time_;}

bool Device::is_on() const {return is_on_;}

void Device::switch_on(const Time& current_time) {
    if(!is_on_){
        last_activation_time = std::make_unique<Time>(current_time);
    }
    is_on_ = true;
}

void Device::switch_off(const Time& current_time) {
    if(is_on_ && (last_activation_time && *last_activation_time <= current_time)){
        total_power_ += calculateTimePeriodPower(*last_activation_time, current_time, KPower);
    }
    last_activation_time.reset();
    
    is_on_ = false;
}

double Device::get_total_power(const Time& current_time) const{
    if(is_on_ && (last_activation_time && *last_activation_time <= current_time)){
        return total_power_ + calculateTimePeriodPower(*last_activation_time, current_time, KPower);
    }
    return total_power_;
}

void Device::removeTimer(){
    start_time_.reset();
}

void Device::resetDevice(){
    total_power_ = 0;
    last_activation_time.reset();
    is_on_ = false;
}

void Device::resetDeviceAndTimer(){
    resetDevice();
    removeTimer();
}

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

static double calculateTimePeriodPower(const Time& start_time, const Time& stop_time, const double power){
    if(start_time > stop_time)
        throw std::invalid_argument("Start time is higher than stop time");

    Time active_time = stop_time - start_time;
    return ((active_time.getHour() * power) + ((power / 60) * active_time.getMinute()));
}