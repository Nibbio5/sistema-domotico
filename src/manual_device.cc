// Eros Menin

#include "../include/manual_device.h"

ManualDevice::ManualDevice(const std::string& name, const double power, const Time& startTime, const Time& stopTime) : Device(name, power, startTime) {
    if(stopTime < startTime)
        throw std::invalid_argument("start time is higher than stop time");

    this->stopTime = std::make_shared<Time>(stopTime);
}

ManualDevice::ManualDevice(const std::string& name, const double power, const Time& startTime) : Device(name, power, startTime), stopTime{nullptr} {}

ManualDevice::ManualDevice(const std::string& name, const double power) : Device(name, power), stopTime{nullptr} {}

std::shared_ptr<const Time> ManualDevice::getStopTime() const {
    return stopTime;
}

void ManualDevice::setNewTimer(const Time& newStartTime, const Time& newStopTime) {
    if(newStopTime < newStartTime)
        throw std::invalid_argument("start time is higher than stop time");

    auto newStartTime_ptr = std::make_shared<Time>(newStartTime);
    auto newStopTime_ptr = std::make_shared<Time>(newStopTime);
    startTime = newStartTime_ptr;
    stopTime = newStopTime_ptr;
}

// Time ManualDevice::getActivityDuration() const {
//     int totMinutes = getActivityDurationInMinutes();
//     return Time(floor(totMinutes / 60), (totMinutes % 60));
// }

// int ManualDevice::getActivityDurationInMinutes() const {
//     return ((*stopTime.getHour() - *startTime.getHour()) * 60) + (stopTime.getMinute() - startTime.getMinute());
// }

std::ostream& operator<<(std::ostream& out, const ManualDevice& device) {
    out << "ManualDevice{"
        << "id=" << device.id
        << ", name=" << device.name
        << ", power=" << device.power
        << ", isOn=" << device.isDeviceOn();

    out << ", startTime=";
    if(device.getStartTime()) {
        out << *device.getStartTime();
    } else {
        out << "NOT_SET";
    }
    out << ", stopTime=";
    if(device.getStopTime()) {
        out << *device.getStopTime();
    } else {
        out << "NOT_SET";
    }
    out << "}";

    return out;
}
