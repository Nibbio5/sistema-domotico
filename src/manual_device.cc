#include "..\include\manual_device.h"
#include <string>
#include <ostream>
#include <exception>
#include <cmath>

ManualDevice::ManualDevice(std::string name, double power, Time startTime, Time stopTime) : Device(name, power) {
    if(stopTime < startTime)
        throw std::invalid_argument("start time is higher than stop time");

    this->startTime = startTime;
    this->stopTime = stopTime;
}

void ManualDevice::setNewTimer(const Time& newStartTime, const Time& newStopTime){
    if(newStopTime < newStartTime)
        throw std::invalid_argument("start time is higher than stop time");
    
    startTime = Time(newStartTime);
    stopTime= Time(newStopTime);
}

Time ManualDevice::getActivityDuration() const {
    int totMinutes = getActivityDurationInMinutes();
    return Time(floor(totMinutes / 60), (totMinutes % 60));
}

int ManualDevice::getActivityDurationInMinutes() const {
    if(stopTime < startTime)
        throw std::invalid_argument("start time is higher than stop time");
    
    return ((stopTime.getHour() - startTime.getHour()) * 60) + (stopTime.getMinute() - startTime.getMinute());
}

std::ostream& operator<<(std::ostream& out, const ManualDevice device) {
    return out << "ManualDevice{"
    << "id=" << device.id
    << ", name=" << device.name
    << ", power=" << device.power
    << ", isOn=" << device.isDeviceOn()
    << ", startTime" << device.getStartTime()
    << ", stopTime" << device.getStopTime()
    << "}";
}