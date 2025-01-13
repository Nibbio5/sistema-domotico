/**
 * @file terminal.cc
 * @author Francesco Dotoli (francesco.dotoli@studenti.unipd.it)
 * @brief part of implementation of the terminal class
 * @date 2024-12-27
 *
 */

#include "../include/terminal.h"

Terminal::Terminal(double power_limit)
    : domotics_system_(power_limit), log_(report::logs::getInstance())  {}

void Terminal::setCommandPrompt(const std::vector<std::string> &args) {
    if(args.size() < 2) {
        throw std::invalid_argument("Argomento sbagliato. Scrivere 'help' per maggiori informazioni.");
    }

    try {
        if(args[0] == "time") {
            setTimeCommandPrompt(args[1]);
            return;
        }
        Device *device = isDevice(args.at(0));
        if(device == nullptr) {
            throw std::invalid_argument("Dispositivo sbagliato. Scrivere 'help' per maggiori informazioni.");
        }
        setDeviceCommandPrompt(args, device);
    } catch(const std::out_of_range &e) {
        throw std::invalid_argument(e.what());
    }
}

void Terminal::setDeviceCommandPrompt(
    const std::vector<std::string> &args,
    Device *device
) {
    if(args.at(1) == "on" || args.at(1) == "off") {
        domotics_system_.changeDeviceStatus((args.at(1) == "on"), device->KName);
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
        throw std::invalid_argument("Argomento sbagliato. Scrivere 'help' per maggiori informazioni.");
    }
    Time start_time = Time::fromString(start);
    Time stop_time = (stop.empty()) ? Time(23, 59) : Time::fromString(stop);

    Device *device = isDevice(device_name);
    if(device == nullptr) {
        throw std::invalid_argument("Argomento sbagliato. Scrivere 'help' per maggiori informazioni.");
    }
    domotics_system_.setDeviceTime(device->KName, start_time, stop_time);
}

void Terminal::rmCommandPrompt(const std::string &arg) {
    if(arg.empty()) {
        throw std::invalid_argument("Argomento sbagliato. Scrivere 'help' per maggiori informazioni.");
    }

    const Device *device = isDevice(arg);
    if(device == nullptr) {
        throw std::invalid_argument("Dispositivo sbagliato. Scrivere 'help' per maggiori informazioni.");
    }

    domotics_system_.removeDeviceTimer(device->KName);
}

void Terminal::showCommandPrompt(const std::string &arg) {
    if(!arg.empty()) {
        const Device *device = isDevice(arg);
        if(device == nullptr) {
            throw std::invalid_argument("Dispositivo sbagliato. Scrivere 'help' per maggiori informazioni.");
        }
        std::string output = showOneDevice(device);
        log_.addLog(report::message(domotics_system_.getCurrentTime(), output));
        return;
    }

    double consumed_power = 0.0;
    double produced_power = 0.0;
    std::string output = "Nello specifico:\n";
    std::vector<Device *> devices = domotics_system_.getDevices();
    for(const Device *device : devices) {
        if(device->KPower >= 0) {
            produced_power += device->get_total_power(domotics_system_.getCurrentTime());
        } else {
            consumed_power += device->get_total_power(domotics_system_.getCurrentTime());
        }
        output = output + showOneDevice(device, false);
    }
    Time currentTime = domotics_system_.getCurrentTime();
    std::string currentTimeString = currentTime.getHourString() + ":" + currentTime.getMinuteString();
    output  = "Attualmente il sistema ha prodotto " +
              roundTo(produced_power) + "kWh e consumato " +
              roundTo(consumed_power) + "kWh. " + output;
    log_.addLog(report::message(currentTime, output));
}

std::string Terminal::showOneDevice(const Device *device, const bool &isSigle) {

    Time currentTime = domotics_system_.getCurrentTime();
    std::string startPhrase = isSigle ? "" : "- ";
    std::string powerStatus = device->is_on() ? "on" : "off";

    return startPhrase + "Il dispositivo " + device->KName + " (" + powerStatus + ")" + " ha " + (isSigle ? "attualmente " : "")
           + (device->KPower >= 0 ? "prodotto " : "consumato ")
           + roundTo(device->get_total_power(domotics_system_.getCurrentTime())) + "kWh\n";
}

void Terminal::setTimeCommandPrompt(const std::string &arg) {
    if(arg.empty()) {
        throw std::invalid_argument("Argomento sbagliato. Scrivere 'help' per maggiori informazioni.");
    }

    try {
        Time time = Time::fromString(arg);

        domotics_system_.setCurrentTime(time);
    } catch(const std::invalid_argument &e) {
        throw std::invalid_argument(e.what());
    }
}

Device *Terminal::isDevice(const std::string &arg) {
    for(Device *device : domotics_system_.getDevices()) {
        if(device->KName == arg) {
            return device;
        }
    }
    return nullptr;
}

void Terminal::resetCommandPrompt(const std::string &arg) {
    if(arg.empty()) {
        throw std::invalid_argument("Argomento sbagliato. Scrivere 'help' per maggiori informazioni.");
    }

    if(arg == "time") {
        domotics_system_.resetTime();
    } else if(arg == "timers") {
        domotics_system_.resetTimers();
    } else if(arg == "all") {
        domotics_system_.resetAll();
    } else {
        throw std::invalid_argument("Argomento sbagliato. Scrivere 'help' per maggiori informazioni.");
    }
}

void Terminal::helpCommandPrompt() {
    std::cout << "Commands:\n"
              << " - set \"{device}\" {start} [{stop}]: Imposta l'orario di accensione [e spegnimento(solo M)] per il dispositivo.\n"
              << " - set \"{device}\" on/off: accende/spegne il dispositivo\n"
              << " - rm \"{device}\": rimuove il timer del dispositivo\n"
              << " - show \"{device}\": mostra lo stato del dispositivo, se non è specificato il dispositivo li mostra tutti\n"
              << " - reset time: riporta il tempo a 00:00 spegnendo i dispositivi ma mantenendo i timer\n"
              << " - reset timers: rimuove i timer dai dispositivi (mantengono lo stato)\n"
              << " - reset all: riporta il sistema allo stato iniziale\n"
              << " - exit: esci dalla casa\n";
}

static std::string roundTo(double value, int precision) {
    std::string value_str = std::to_string(std::round(value * std::pow(10, (precision + 1))) / std::pow(10, (precision + 1)));
    int pos = value_str.find(".");
    std::string result = value_str.substr(0, pos) + ".";

    for(int i = (pos + 1); i < (pos + 1) + precision; i++) {
        result += value_str[i];
    }
    return result;
}
