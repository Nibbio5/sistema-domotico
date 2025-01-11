#include "../include/domotics_system.h"
#include <memory>

DomoticsSystem::DomoticsSystem() : KPowerLimit{3.5}, currentTime{Time(0, 0)}, powerLoad{0}, log{report::logs::getInstance()} {
    setDevices();
    orderByStartTime();
    checkSchedule();
}

DomoticsSystem::DomoticsSystem(double power) : KPowerLimit{power}, currentTime{Time(0, 0)}, powerLoad{0}, log{report::logs::getInstance()} {
    setDevices();
    orderByStartTime();
    checkSchedule();
}

void DomoticsSystem::setDevices() {
    std::ifstream file("./assets/devices.json");
    if(!file) {
        throw std::invalid_argument("il file devices.json non esiste");
    }
    std::string line;
    std::string nome;
    std::string durataAccensione;
    std::string produzioneConsumo;
    bool isOnWhiteList = false;
    std::shared_ptr<Time> orarioAccensione = std::shared_ptr<Time>(nullptr);
    std::shared_ptr<Time> orarioSpegnimento = std::shared_ptr<Time>(nullptr);

    // manual parsing of the JSON (simple)
    while(std::getline(file, line)) {
        if(line.find("\"Nome\":") != std::string::npos) {
            nome = line.substr(line.find(":") + 2);
            nome = nome.substr(1, nome.size() - 3); // Remove the quotes and the final comma
        } else if(line.find("\"DurataAccensione\":") != std::string::npos) {
            durataAccensione = line.substr(line.find(":") + 2);
            durataAccensione = durataAccensione.substr(1, durataAccensione.size() - 3);
        } else if(line.find("\"OrarioAccensione\":") != std::string::npos) {
            orarioAccensione = std::make_shared<Time>(Time::fromString(line.substr(line.find(":") + 2)));
        } else if(line.find("\"OrarioSpegnimento\":") != std::string::npos) {
            orarioSpegnimento = std::make_shared<Time>(Time::fromString(line.substr(line.find(":") + 2)));
        } else if(line.find("\"WhiteList\":") != std::string::npos) {
            isOnWhiteList = line.find("true") != std::string::npos;
        } else if(line.find("\"ProduzioneConsumo\":") != std::string::npos) {
            produzioneConsumo = line.substr(line.find(":") + 2);
            produzioneConsumo = produzioneConsumo.substr(1, produzioneConsumo.size() - 3);
            // Create the device based on the durataAccensione
            if(durataAccensione == "Manuale") {
                if(orarioAccensione != nullptr) {
                    if(orarioSpegnimento == nullptr) {
                        ManualDevice *device = new ManualDevice(nome, std::stod(produzioneConsumo), *orarioAccensione.get(), isOnWhiteList);
                        active_devices.push_back(device);
                        all_devices.push_back(device);
                    } else {
                        ManualDevice *device = new ManualDevice(nome, std::stod(produzioneConsumo), *orarioAccensione.get(), *orarioSpegnimento.get(), isOnWhiteList);
                        active_devices.push_back(device);
                        all_devices.push_back(device);
                    }
                    orarioAccensione = nullptr;
                    orarioSpegnimento = nullptr;
                } else {
                    all_devices.push_back(new ManualDevice(nome, std::stod(produzioneConsumo)));
                }
            } else {
                if(orarioAccensione != nullptr) {
                    CPDevice *device = new CPDevice(nome, std::stod(produzioneConsumo), *orarioAccensione.get(), isOnWhiteList);
                    active_devices.push_back(device);
                    all_devices.push_back(device);
                }
                orarioAccensione = std::make_shared<Time>(Time::fromString(durataAccensione));
                all_devices.push_back(new CPDevice(nome, std::stod(produzioneConsumo), *orarioAccensione.get(), isOnWhiteList));
                orarioAccensione = nullptr;
            }
        }
        isOnWhiteList = false;
    }
}

std::vector<Device *> DomoticsSystem::getDevices() const {
    return all_devices;
}

double DomoticsSystem::getPowerLoad() const {
    return powerLoad;
}

Time DomoticsSystem::getCurrentTime() const {
    return currentTime;
}

void DomoticsSystem::removeDeviceTimer(std::string device) {
    int index = getIndex(device, false);
    int activeIndex = getIndex(device, true);
    if(index == -1) {
        throw std::invalid_argument("device not found or already removed");
    }
    if(dynamic_cast<ManualDevice *>(active_devices[activeIndex]) != nullptr) {
        dynamic_cast<ManualDevice *>(active_devices[activeIndex])->removeTimer();
        dynamic_cast<ManualDevice *>(all_devices[index])->removeTimer();
    } else if(dynamic_cast<CPDevice *>(active_devices[activeIndex]) != nullptr) {
        dynamic_cast<CPDevice *>(active_devices[activeIndex])->removeTimer();
        dynamic_cast<CPDevice *>(all_devices[index])->removeTimer();
    }
    checkSchedule();
}

int DomoticsSystem::getIndex(std::string device, bool prova) const {
    std::vector<Device *> devices;
    (!prova) ? devices = all_devices : devices = active_devices;
    for(int i = 0; i < devices.size(); i++) {
        if(devices[i]->KName == device)
            return i;
    }
    return -1;
}

bool DomoticsSystem::isPresent(std::string device) const {
    return getIndex(device, true) != -1;
}

void DomoticsSystem::checkSchedule() {
    Time tempo = Time(0, 0);
    for(int i = 0; i <= active_devices.size();) {
        if(i < active_devices.size()) {
            if(*active_devices[i]->get_start_time().get() <= currentTime)
                tempo = *active_devices[i]->get_start_time().get();
        } else
            tempo = currentTime;

        bool deviceRemoved = false;
        for(auto it = active_devices.begin(); it != active_devices.end(); it++) {
            Device *device = *it;
            std::shared_ptr<const Time> startTime = device->get_start_time();
            if(ManualDevice *manualDevice = dynamic_cast<ManualDevice *>(device)) {
                std::shared_ptr<const Time> stopTime = manualDevice->get_stop_time();

                if(startTime != nullptr && *startTime.get() <= tempo && stopTime != nullptr && *stopTime > tempo && !manualDevice->is_on()) {
                    manualDevice->switch_on(*startTime);
                    powerLoad += manualDevice->KPower;
                    log.addLog(report::message(*startTime, "Il dispositivo " + manualDevice->KName + " si é acceso"));
                } else if(stopTime != nullptr && (*stopTime <= tempo && manualDevice->is_on())) {
                    log.addLog(report::message(*stopTime, "Il dispositivo " + manualDevice->KName + " si é spento"));
                    manualDevice->switch_off(tempo);
                    powerLoad -= manualDevice->KPower;
                    it = active_devices.erase(it);
                    deviceRemoved = true;
                    continue;
                }
            } else if(CPDevice *cpDevice = dynamic_cast<CPDevice *>(device)) {
                Time duration = cpDevice->KDuration;
                if(startTime != nullptr && !cpDevice->is_on() && *startTime <= tempo && *startTime + duration >= tempo) {
                    cpDevice->switch_on(*startTime);
                    powerLoad += cpDevice->KPower;
                    log.addLog(report::message(*startTime, "Il dispositivo " + cpDevice->KName + " si é acceso"));
                }
                if(startTime != nullptr && tempo >= *startTime + duration && cpDevice->is_on()) {
                    log.addLog(report::message(duration + *startTime, "Il dispositivo " + cpDevice->KName + " si é spento"));
                    cpDevice->switch_off(duration + *startTime);
                    powerLoad -= cpDevice->KPower;
                    it = active_devices.erase(it);
                    deviceRemoved = true;
                    continue;
                }
            }
        }
        if(!deviceRemoved) {
            ++i;
        }
    }
}

// void checkScheduleManualDevice(ManualDevice *manualDevice, Time currentTime, const Time *startTime, const Time *lastActivationTime, double &powerLoad) {
//     auto stopTime = manualDevice->get_stop_time();
//     if(startTime != nullptr && *startTime <= currentTime && stopTime != nullptr && *stopTime > currentTime && !manualDevice->is_on()) {
//         manualDevice->switch_on(*startTime);
//         powerLoad += manualDevice->KPower;
//         // log(*startTime, "Il dispositivo " + manualDevice->KName + " si é acceso");
//     } else if(stopTime != nullptr &&
//               (*stopTime <= currentTime && manualDevice->is_on() || lastActivationTime != nullptr && currentTime < *lastActivationTime && manualDevice->is_on())) {
//         // log(*stopTime, "Il dispositivo " + manualDevice->KName + " si é spento");
//         manualDevice->switch_off(currentTime);
//         powerLoad -= manualDevice->KPower;
//     }
// }

// void checkScheduleCpDevice(CPDevice *cpDevice, Time currentTime, const Time *startTime, const Time *lastActivationTime, double &powerLoad) {
//     auto duration = cpDevice->KDuration;
//     if(startTime != nullptr && !cpDevice->is_on() && *startTime <= currentTime && *startTime + duration > currentTime) {
//         cpDevice->switch_on(*startTime);
//         powerLoad += cpDevice->KPower;
//         // log(*startTime, "Il dispositivo " + cpDevice->KName + " si é acceso");
//     }
//     if(lastActivationTime != nullptr && currentTime >= duration + *lastActivationTime && cpDevice->is_on() || startTime != nullptr && currentTime < *startTime && cpDevice->is_on()) {
//         // log(duration + *lastActivationTime, "Il dispositivo " + cpDevice->KName + " si é spento");
//         cpDevice->switch_off(duration + *lastActivationTime);
//         powerLoad -= cpDevice->KPower;
//     }
// }

void DomoticsSystem::orderByStartTime() {
    std::sort(active_devices.begin(), active_devices.end(), [](Device * a, Device * b) {
        std::shared_ptr<const Time> startTimeA = a->get_start_time();
        std::shared_ptr<const Time> startTimeB = b->get_start_time();
        if(startTimeA == nullptr && startTimeB == nullptr) return false;
        if(startTimeA == nullptr) return false;
        if(startTimeB == nullptr) return true;
        return *startTimeA < *startTimeB;
    });
    // Move devices with nullptr start time to the end
    std::stable_partition(active_devices.begin(), active_devices.end(), [](Device * device) {
        return device->get_start_time() != nullptr;
    });
}

void DomoticsSystem::setCurrentTime(const Time &newTime) {
    (newTime < currentTime) ? throw std::invalid_argument("time is lower than current time") : currentTime = newTime;
    orderByStartTime();
    checkSchedule();
    log.addLog(report::message(currentTime, "L'orario attuale è " + currentTime.getHourString() + ":" + currentTime.getMinuteString()));
}

void DomoticsSystem::changeDeviceStatus(bool status, std::string device) {
    int index = getIndex(device, false);
    if(index == -1) {
        throw std::invalid_argument("device not found");
    }
    int activeIndex = getIndex(device, true);
    if(activeIndex == -1 && status) {
        active_devices.push_back(all_devices[index]);
        active_devices.back()->switch_on(currentTime);
        active_devices.back()->set_start_time(currentTime);
        log.addLog(report::message(currentTime, "Il dispositivo " + device + " si é acceso"));
        powerLoad += active_devices.back()->KPower;
        balancePower(device);
    } else if(activeIndex != -1 && !status) {
        active_devices[activeIndex]->switch_off(currentTime);
        powerLoad -= active_devices[activeIndex]->KPower;
        log.addLog(report::message(currentTime, "Il dispositivo " + device + " si é spento"));
    } else {
        active_devices[activeIndex]->switch_on(currentTime);
        active_devices[activeIndex]->set_start_time(currentTime);
        powerLoad += active_devices[activeIndex]->KPower;
        log.addLog(report::message(currentTime, "Il dispositivo " + device + " si é acceso"));
        balancePower(device);
    }
}

void DomoticsSystem::balancePower(std::string last) {
    Time latestTime(0, 0);
    std::string name;
    while(-powerLoad > KPowerLimit) {
        log.addLog(report::message(currentTime, "Il sistema è in sovraccarico energetico, gli ultimi dispositivi accesi verranno spenti"));
        for(int i = 0; i < active_devices.size(); i++) {
            if(active_devices[i]->get_last_activation_time() != nullptr &&
                    !active_devices[i]->KIsOnWhiteList &&
                    *active_devices[i]->get_last_activation_time() >= latestTime &&
                    active_devices[i]->KName != last &&
                    active_devices[i]->is_on()) {
                latestTime = *active_devices[i]->get_last_activation_time();
                name = active_devices[i]->KName;
            }
        }
        int activeIndex = getIndex(name, true);
        if(activeIndex != -1) {
            log.addLog(report::message(currentTime, "Il dispositivo " + name + " si é spento"));
            active_devices[activeIndex]->switch_off(currentTime);

            powerLoad -= active_devices[activeIndex]->KPower;
        }
        latestTime = Time(0, 0); // Reset latestTime for the next iteration
    }
}
void DomoticsSystem::setDeviceTime(const std::string &device, const Time &start, const Time &stop) {
    int index = getIndex(device, false);
    if(index == -1 || start > stop || start < currentTime || stop < currentTime) {
        throw std::invalid_argument("Invalid input data, please check the device name and the time");
    }

    int activeIndex = getIndex(device, true);
    Device *targetDevice = all_devices[index];

    if(activeIndex == -1) {
        active_devices.push_back(targetDevice);
    } else {
        targetDevice = active_devices[activeIndex];
    }

    if(CPDevice *cpDevice = dynamic_cast<CPDevice *>(targetDevice)) {
        cpDevice->set_start_time(start);
    } else if(ManualDevice *manualDevice = dynamic_cast<ManualDevice *>(targetDevice)) {
        manualDevice->set_new_timer(start, stop);
    }
    checkSchedule();
}

std::ostream &operator<<(std::ostream &out, const DomoticsSystem &sys) {
    for(auto value : sys.getDevices()) {
        if(CPDevice *cpDevice = dynamic_cast<CPDevice *>(value)) {
            out << *cpDevice << std::endl;
        } else if(ManualDevice *manualDevice = dynamic_cast<ManualDevice *>(value)) {
            out << *manualDevice << std::endl;
        }
    }
    out << "Current time: " << sys.getCurrentTime() << std::endl;
    out << "Power load: " << sys.getPowerLoad() << std::endl;
    return out;
}

std::vector<Device *> DomoticsSystem::getDevicesVector() const {
    std::vector<Device *> deviceList;
    for(auto *value : active_devices) {
        deviceList.push_back(value);
    }
    return deviceList;
}

void DomoticsSystem::resetTime() {
    currentTime = Time(0, 0);
    for(auto *value : all_devices) {
        if(value->get_start_time() != nullptr && getIndex(value->KName, true) == -1) {
            active_devices.push_back(value);
        }
        value->resetDevice();
    }
    orderByStartTime();
    powerLoad = 0;
    checkSchedule();
}

void DomoticsSystem::resetTimers() {
    // to be finished
    for(auto *value : active_devices) {
        if(ManualDevice *manualDevice = dynamic_cast<ManualDevice *>(value)) {
            manualDevice->removeTimer();
        } else {
            value->removeTimer();
        }
    }
    checkSchedule();
}

void DomoticsSystem::resetAll() {
    currentTime = Time(0, 0);
    for(auto *value : all_devices) {
        value->resetDeviceAndTimer();
    }
    active_devices.clear();
    powerLoad = 0;
    checkSchedule();
}
