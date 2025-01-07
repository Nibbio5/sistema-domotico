// Eros Menin

#include "../include/manual_device.h"

ManualDevice::ManualDevice(const std::string& name, const double power, const Time& start_time, const Time& stop_time) : Device(name, power, start_time) {
    if(stop_time < start_time)
        throw std::invalid_argument("Start time is higher than stop time");

    stop_time_ = std::make_shared<Time>(stop_time);
}

ManualDevice::ManualDevice(const std::string& name, const double power, const Time& startTime) : Device(name, power, startTime), stop_time_{nullptr} {}

ManualDevice::ManualDevice(const std::string& name, const double power) : Device(name, power), stop_time_{nullptr} {}

std::shared_ptr<const Time> ManualDevice::get_stop_time() const {return stop_time_;}

void ManualDevice::set_new_timer(const Time& newStartTime, const Time& newStopTime) {
    if(newStopTime < newStartTime)
        throw std::invalid_argument("Start time is higher than stop time");

    start_time_ = std::make_shared<Time>(newStartTime);
    stop_time_ = std::make_shared<Time>(newStopTime);
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
    << "id=" << device.KId
    << ", name=" << device.KName
    << ", power=" << device.KPower
    << ", is_on=" << device.is_on_;
    
    out << ", start_time=";
    if(device.start_time_) {
        out << *device.start_time_;
    } else {
        out << "NOT_SET";
    }
    out << ", stop_time=";
    if(device.stop_time_) {
        out << *device.stop_time_;
    } else {
        out << "NOT_SET";
    }
    out << "}";

    return out;
}
