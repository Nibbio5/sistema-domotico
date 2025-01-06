// Eros Menin

#include "../include/device.h"

Device::Device(const std::string& name, const double power, const Time& startTime) : id{SingletonId::getInstance().generateId()}, name(name), power(power), isOn{false}, startTime{std::make_shared<Time>(startTime)} {}

Device::Device(const std::string& name, const double power) : id{SingletonId::getInstance().generateId()}, name(name), power(power), isOn{false}, startTime{nullptr} {}

std::shared_ptr<const Time> Device::getStartTime() const {return startTime;}

bool Device::isDeviceOn() const {return isOn;}

void Device::switchOn() {isOn = true;}

void Device::switchOff() {isOn = false;}

std::ostream& operator<<(std::ostream& out, const Device& device) {
    out << "Device{"
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

    out << "}";

    return out;
}