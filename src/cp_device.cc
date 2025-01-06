// Eros Menin

#include "../include/cp_device.h"
#include <ostream>
#include <string>

CPDevice::CPDevice(std::string name, double power, Time startTime,
                   Time duration)
    : duration{duration}, Device(name, power, startTime) {}

CPDevice::CPDevice(std::string name, double power, Time duration)
    : duration{duration}, Device(name, power, Time()) {}

void CPDevice::setStartTime(Time newTime) { startTime = newTime; }

std::ostream &operator<<(std::ostream &out, const CPDevice device) {
  return out << "CPDevice{"
             << "id=" << device.id << ", name=" << device.name
             << ", power=" << device.power << ", isOn=" << device.isDeviceOn()
             << ", startTime=" << device.getStartTime()
             << ", duration=" << device.duration << "}";
}
