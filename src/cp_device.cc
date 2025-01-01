#include "..\include\cp_device.h"
#include <string>
#include <chrono>
#include <ostream>

CPDevice::CPDevice(std::string name, double power, std::chrono::seconds duration) : duration{duration}, Device(name, power){}

std::ostream& operator<<(std::ostream& out, const CPDevice device) {
        return out << "CPDevice{"
        << "id=" << device.id
        << ", name=" << device.name
        << ", power=" << device.power
        << ", isOn=" << device.isDeviceOn()
        << ", duration" << device.duration.count()
        << "}";
}