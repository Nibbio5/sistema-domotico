#include "../include/terminal.h"
#include "../include/device.h"
#include "../include/time.h"

Terminal::Terminal() {}

Terminal::~Terminal() {}

void Terminal::setCommandPrompt(const std::vector<std::string> &args) {
  if (args.size() < 2) {
    throw std::invalid_argument(
        "Invalid arguments provided. Type 'help' for more information.");
  }

  if (args[0] == "time") {
    setTimeCommandPrompt(args.at(1));
  }
  Device *device = isDevice(args.at(1));
  if (device == nullptr) {
    throw std::invalid_argument(
        "Invalid device provided. Type 'help' for more information.");
  }
}

void Terminal::setDeviceCommandPrompt(const std::vector<std::string> &args,
                                      Device *device) {
  if (args.size() < 3) {
    throw std::invalid_argument(
        "Invalid arguments provided. Type 'help' for more information.");
  } else if (args[2] == "on") {
    device->switchOn();
  } else if (args[2] == "off") {
    device->switchOff();
  } else {
    // TODO: call the timer setter for the device
  }
}

void Terminal::rmCommandPrompt(const std::string &arg) {
  if (arg.empty()) {
    throw std::invalid_argument(
        "Invalid arguments provided. Type 'help' for more information.");
  }

  Device *device = isDevice(arg);
  if (device == nullptr) {
    throw std::invalid_argument(
        "Invalid device provided. Type 'help' for more information.");
  }

  device->remove();
}

void Terminal::setTimeCommandPrompt(const std::string &arg) {
  if (arg.empty()) {
    throw std::invalid_argument(
        "Invalid arguments provided. Type 'help' for more information.");
  }

  Time time = Time::fromString(arg);

  // TODO:call the time setter
}

Device *Terminal::isDevice(const std::string &arg) {
  for (Device *device : domotics_system.getDevices()) {
    if (device->name == arg) {
      return device;
    }
  }
  return nullptr;
}
