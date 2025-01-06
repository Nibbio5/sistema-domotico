// Eros Menin

#include "../include/manual_device.h"

ManualDevice::ManualDevice(const std::string& name, const double power, const Time& startTime, const Time& stopTime) : Device(name, power, startTime) {
    if(stopTime < startTime)
        throw std::invalid_argument("start time is higher than stop time");

    *(this->stop_time_) = stopTime;
}

ManualDevice::ManualDevice(const std::string& name, const double power, const Time& startTime) : Device(name, power, startTime), stop_time_{nullptr} {}

ManualDevice::ManualDevice(const std::string& name, const double power) : Device(name, power), stop_time_{nullptr} {}

std::shared_ptr<const Time> ManualDevice::getStopTime() const {return stop_time_;}

void ManualDevice::setNewTimer(const Time& newStartTime, const Time& newStopTime) {
    if(newStopTime < newStartTime)
        throw std::invalid_argument("start time is higher than stop time");

    startTime = std::make_shared<Time>(newStartTime);
    stopTime = std::make_shared<Time>(newStopTime);
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
<<<<<<< HEAD
    << "id=" << device.kid_
    << ", name=" << device.kname_
    << ", power=" << device.kpower_
    << ", isOn=" << device.isOn();
    
=======
        << "id=" << device.id
        << ", name=" << device.name
        << ", power=" << device.power
        << ", isOn=" << device.isDeviceOn();

>>>>>>> Francesco
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
