#include "../include/terminal.h"
#include <iostream>
#include <string>

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

void Terminal::setDeviceCommandPrompt(
    const std::vector<std::string> &args,
    Device *device
) {
    if(args.at(1) == "on" || args.at(1) == "off") {
        domotics_system.changeDeviceStatus((args.at(1) == "on"), device->KName);
        // device->switch_on();// device->switch_off();
    } else {
        std::string start = args.at(1);
        std::string stop = args.size() > 2 ? args.at(2) : "";
        setDeviceTimer(device->KName, start, stop);
    }
}

void Terminal::setDeviceTimer(
    const std::string &device_name,
    const std::string &start,
    const std::string &stop
) {
    if(start.empty()) {
        throw std::invalid_argument(
            "Invalid arguments provided. Type 'help' for more information.");
    }
    Time start_time = Time::fromString(start);
    Time stop_time = (stop.empty()) ? Time() : Time::fromString(stop);

    Device *device = isDevice(device_name);
    if(device == nullptr) {
        throw std::invalid_argument(
            "Invalid device provided. Type 'help' for more information.");
    }
    // TODO: implement setDeviceTime method from DomoticsSystem
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

    domotics_system.removeDevice(device->KName);
}

void Terminal::showCommandPrompt(const std::string &arg) {
    if(!arg.empty()) {
        showOneDevice(arg);
        return;
    }

    double consumed_power = 0.0;
    double produced_power = 0.0;
    std::string output = "Nello specifico:\n";
    std::vector<Device *> devices = domotics_system.getDevices();
    for(const Device *device : devices) {
        if(device->KName == "Impianto Fotovoltaico") {
            produced_power += device->KPower;
        } else {
            consumed_power += device->KPower;
        }
        output = output + "- " + device->KName + "ha " +
                 (device->KName == "Impianto Fotovoltaico" ? "prodotto " : "consumato ") +
                 std::to_string(device->KPower) + "kWh\n";
    }
    Time currentTime = domotics_system.getCurrentTime();
    std::cout << "[" << currentTime << "] Attualmente il sistema ha prodotto " +
              std::to_string(produced_power) + "kWh e consumato " +
              std::to_string(consumed_power) + "kWh. " + output;
}

void Terminal::showOneDevice(const std::string &arg) {

    const Device *device = isDevice(arg);
    if(device == nullptr) {
        throw std::invalid_argument(
            "Invalid device provided. Type 'help' for more information.");
    }
    Time currentTime = domotics_system.getCurrentTime();
    std::cout << "[" << currentTime << "] Il dispositivo " << device->KName << " ha attualmente "
              << (device->KName == "Impianto Fotovoltaico" ? "prodotto "
                  : "consumato ")
              << device->KPower << "kWh\n";
}

void Terminal::setTimeCommandPrompt(const std::string &arg) {
    if(arg.empty()) {
        throw std::invalid_argument(
            "Invalid arguments provided. Type 'help' for more information.");
    }

    Time time = Time::fromString(arg);

    try {
        domotics_system.setCurrentTime(time);
    } catch(const std::invalid_argument &e) {
        std::cerr << e.what() << std::endl;
    }
}

Device *Terminal::isDevice(const std::string &arg) {
    for(Device *device : domotics_system.getDevices()) {
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

    if(arg == "time") {
        domotics_system.resetTime();
    } else if(arg == "timers") {
        domotics_system.resetTimers();
    } else if(arg == "all") {
        domotics_system.resetAll();
    } else {
        throw std::invalid_argument(
            "Invalid argument provided. Type 'help' for more information.");
    }
}
