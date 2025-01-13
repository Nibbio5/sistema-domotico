/**
 * @file manual_device.cc
 * @brief Implementation file for the ManualDevice class.
 *
 * @author Eros Menin
 *
 */

#include "../include/manual_device.h"

ManualDevice::ManualDevice(const std::string& name, const double power, const Time& start_time, const Time& stop_time, const bool is_on_white_list) 
    : Device(name, power, start_time, is_on_white_list) {
    if(start_time > stop_time)
        throw std::invalid_argument("Il tempo di start è successivo al tempo di stop");

    this->stop_time_ = std::make_shared<Time>(stop_time);
}

ManualDevice::ManualDevice(const std::string& name, const double power, const Time& start_time, const bool is_on_white_list) 
    : Device(name, power, start_time, is_on_white_list) {}

ManualDevice::ManualDevice(const std::string& name, const double power) : Device(name, power) {}

std::shared_ptr<const Time> ManualDevice::get_stop_time() const { return stop_time_;}

void ManualDevice::set_start_time(const Time& new_time) {
    if(stop_time_ && *stop_time_ < new_time)
        throw std::invalid_argument("Il tempo di start è successivo al tempo di stop");

    start_time_ = std::make_shared<Time>(new_time);
}

void ManualDevice::set_new_timer(const Time& new_start_time, const Time& new_stop_time) {
    if(new_start_time > new_stop_time)
        throw std::invalid_argument("Il tempo di start è successivo al tempo di stop");

    start_time_ = std::make_shared<Time>(new_start_time);
    stop_time_ = std::make_shared<Time>(new_stop_time);
}

void ManualDevice::removeStopTime() {
    stop_time_.reset();
}

void ManualDevice::removeTimer() {
    start_time_.reset();
    stop_time_.reset();
}

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
    
    out << ", is_on_white_list=" << device.KIsOnWhiteList 
        << "}";

    return out;
}