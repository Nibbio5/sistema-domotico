/**
 * @file cp_device.cc
 * @author Eros Menin (eros.menin@studenti.unipd.it)
 * @brief Implementation file for the CPDevice class.
 * @date 2025-01-13
 * 
 */

#include "../include/cp_device.h"

CPDevice::CPDevice(const std::string& name, const double power, const Time& duration, const Time& start_time, const bool is_on_white_list) : Device(name, power, start_time, is_on_white_list), KDuration{duration} {}

CPDevice::CPDevice(const std::string& name, const double power, const Time& duration, const bool is_on_white_list) : Device(name, power, is_on_white_list), KDuration{duration} {}

std::ostream& operator<<(std::ostream& out, const CPDevice& device) {
    out << "CPDevice{"
        << "id=" << device.KId
        << ", name=" << device.KName
        << ", power=" << device.KPower
        << ", is_on=" << device.is_on_
        << ", start_time=";

    if(device.start_time_) {
        out << *device.start_time_;
    } else {
        out << "NOT_SET";
    }

    out << ", duration=" << device.KDuration
        << ", is_on_white_list=" << device.KIsOnWhiteList
        << "}";

    return out;
}