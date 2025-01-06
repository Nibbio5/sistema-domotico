// Eros Menin

#include "../include/manual_device.h"
#include <cmath>
#include <ostream>
#include <string>

ManualDevice::ManualDevice(std::string name, double power, Time startTime,
                           Time stopTime)
    : Device(name, power, startTime) {
  if (stopTime < startTime)
    throw std::invalid_argument("start time is higher than stop time");

  this->stopTime = stopTime;
}

Time ManualDevice::getStopTime() const { return stopTime; }

void ManualDevice::setNewTimer(const Time &newStartTime,
                               const Time &newStopTime) {
  if (newStopTime < newStartTime)
    throw std::invalid_argument("start time is higher than stop time");

  startTime = newStartTime;
  stopTime = newStopTime;
}

Time ManualDevice::getActivityDuration() const {
  int totMinutes = getActivityDurationInMinutes();
  return Time(floor(totMinutes / 60), (totMinutes % 60));
}

int ManualDevice::getActivityDurationInMinutes() const {
  return ((stopTime.getHour() - startTime.getHour()) * 60) +
         (stopTime.getMinute() - startTime.getMinute());
}

std::ostream &operator<<(std::ostream &out, const ManualDevice device) {
  return out << "ManualDevice{"
             << "id=" << device.id << ", name=" << device.name
             << ", power=" << device.power << ", isOn=" << device.isDeviceOn()
             << ", startTime=" << device.getStartTime()
             << ", stopTime=" << device.getStopTime() << "}";
}
