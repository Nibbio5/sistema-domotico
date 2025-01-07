#include "../include/terminal.h"
#include <iostream>

void Terminal::setCommandPrompt(const std::vector<std::string> &args) {
    if(args.size() < 2) {
        throw std::invalid_argument(
            "Invalid arguments provided. Type 'help' for more information.");
    }

    if(args[0] == "time") {
        setTimeCommandPrompt(args.at(1));
        return;
    }
    Device *device = isDevice(args.at(0));
    if(device == nullptr) {
        throw std::invalid_argument(
            "Invalid device provided. Type 'help' for more information.");
    }
    setDeviceCommandPrompt(args, device);
}

void Terminal::setDeviceCommandPrompt(const std::vector<std::string> &args,
                                      Device *device) const {
    if(args.size() < 3) {
        throw std::invalid_argument(
            "Invalid arguments provided. Type 'help' for more information.");
    } else if(args[2] == "on") {
        device->switch_on();
    } else if(args[2] == "off") {
        device->switch_off();
    } else {
        // TODO: call the timer setter for the device
    }
}

void Terminal::rmCommandPrompt(const std::string &arg) {
    if(arg.empty()) {
        throw std::invalid_argument(
            "Invalid arguments provided. Type 'help' for more information.");
    }

    const Device *device = isDevice(arg);
    if(device == nullptr) {
        throw std::invalid_argument(
            "Invalid device provided. Type 'help' for more information.");
    }

    // device->remove(); //TODO: implement remove method
}

void Terminal::showCommandPrompt(const std::string &arg) {
    if(!arg.empty()) {
        showOneDevice(arg);
        return;
    }

    double consumed_power = 0.0;
    double produced_power = 0.0;
    std::string output = "Nello specifico:\n";
    std::vector<Device *> devices = domotics_system.getDevicesVector();
    for(const Device *device : devices) {
        if(device->KName == "Impianto Fotovoltaico") {
            produced_power += device->KPower;
        } else {
            consumed_power += device->KPower;
        }
        output =
            output + "- " + device->KName + "ha " +
            (device->KName == "Impianto Fotovoltaico" ? "prodotto " : "consumato ") +
            std::to_string(device->KPower) + "kWh\n";
    }
    std::cout << "[Time] Attualmente il sistema ha prodotto " +
              std::to_string(produced_power) + "kWh e consumato " +
              std::to_string(consumed_power) + "kWh. " + output;
}

void Terminal::showOneDevice(const std::string &arg) {

    const Device *device = isDevice(arg);
    if(device == nullptr) {
        throw std::invalid_argument(
            "Invalid device provided. Type 'help' for more information.");
    }

    std::cout << "[Time] Il dispositivo " << device->KName << " ha attualmente "
              << (device->KName == "Impianto Fotovoltaico" ? "prodotto "
                  : "consumato ")
              << device->KPower << "kWh\n";
}

void Terminal::setTimeCommandPrompt(const std::string &arg) const {
    if(arg.empty()) {
        throw std::invalid_argument(
            "Invalid arguments provided. Type 'help' for more information.");
    }

    // Time time = Time::fromString(arg);

    // TODO:call the time setter
}

Device *Terminal::isDevice(const std::string &arg) {
    for(Device *device : domotics_system.getDevicesVector()) {
        if(device->KName == arg) {
            return device;
        }
    }
    return nullptr;
}

void Terminal::resetCommandPrompt(const std::string &arg) {
    if(arg.empty()) {
        throw std::invalid_argument(
            "Invalid arguments provided. Type 'help' for more information.");
    }

    const Device *device = isDevice(arg);
    if(device == nullptr) {
        throw std::invalid_argument(
            "Invalid device provided. Type 'help' for more information.");
    }

    // device->reset(); //TODO: implement reset method
}
