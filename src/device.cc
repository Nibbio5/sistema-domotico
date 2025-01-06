// Eros Menin

#include "../include/device.h"
#include <string>

Device::Device(std::string name, double power, Time startTime)
    : id{SingeltonId::getInstance().generateId()}, name(name), power(power),
      isOn{false}, startTime{startTime} {}

Time Device::getStartTime() const { return startTime; }

bool Device::isDeviceOn() const { return isOn; }

void Device::switchOn() { isOn = true; }

void Device::switchOff() { isOn = false; }

std::ostream &operator<<(std::ostream &out, const Device &device) {
  return out << "Device{"
             << "id=" << device.id << ", name=" << device.name
             << ", power=" << device.power << ", isOn=" << device.isDeviceOn()
             << ", startTime=" << device.getStartTime() << "}";
}
