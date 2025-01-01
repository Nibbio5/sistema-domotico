#include "../include/device.h"
#include "../include/singleton_id.h"
#include <string>

Device::Device(std::string name, double power)
    : id{SingeltonId::getInstance().generateId()}, name(name), power(power),
      isOn{false} {}

std::ostream &operator<<(std::ostream &out, const Device &device) {
  return out << "Device{"
             << "id=" << device.id << ", name=" << device.name
             << ", power=" << device.power << ", isOn=" << device.isDeviceOn()
             << "}";
}
