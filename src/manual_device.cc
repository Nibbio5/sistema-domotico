#include "../include/manual_device.h"
#include <string>
#include <chrono>
#include <ostream>

ManualDevice::ManualDevice(std::string name, double power, std::chrono::system_clock::time_point startTime, std::chrono::system_clock::time_point ) : Device(name, power), startTime{startTime}, stopTime{stopTime} {}

std::ostream& operator<<(std::ostream& out, const ManualDevice device) {
        return out << "ManulDevice{"
        << "id=" << device.id
        << ", name=" << device.name
        << ", power=" << device.power
        << ", isOn=" << device.isDeviceOn()
        << ", startTime" << device.getStartTime().time_since_epoch().count()
        << ", stopTime" << device.getStopTime().time_since_epoch().count()
        << "}";
}