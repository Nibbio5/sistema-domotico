#include "../include/domotics_system.h"
#include <memory>
#include <algorithm>

DomoticsSystem::DomoticsSystem() : KPowerLimit{3.5}, currentTime{Time(0, 0)}, powerLoad{0} {
    setDevices();
    checkSchedule();
}

DomoticsSystem::DomoticsSystem(double power) : KPowerLimit{power}, currentTime{Time(0, 0)}, powerLoad{0} {
    setDevices();
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
    auto orarioAccensione = std::shared_ptr<Time>(nullptr);
    auto orarioSpegnimento = std::shared_ptr<Time>(nullptr);

    // manual parsing of the JSON (simple)
    while(std::getline(file, line)) {
        if(line.find("\"Nome\":") != std::string::npos) {
            nome = line.substr(line.find(":") + 2);
            nome = nome.substr(1, nome.size() - 3); // Remove the quotes and the final comma
        } else if(line.find("\"DurataAccensione\":") != std::string::npos) {
            durataAccensione = line.substr(line.find(":") + 2);
            durataAccensione = durataAccensione.substr(1, durataAccensione.size() - 3);
        } else if(line.find("\"OrarioAccensione\":") != std::string::npos) {
            orarioAccensione =  std::make_shared<Time>(Time::fromString(line.substr(line.find(":") + 2)));
        } else if(line.find("\"OrarioSpegnimento\":") != std::string::npos) {
            orarioSpegnimento =  std::make_shared<Time>(Time::fromString(line.substr(line.find(":") + 2)));
        } else if(line.find("\"ProduzioneConsumo\":") != std::string::npos) {
            produzioneConsumo = line.substr(line.find(":") + 2);
            produzioneConsumo = produzioneConsumo.substr(1, produzioneConsumo.size() - 3);
            // Create the device based on the durataAccensione
            if(durataAccensione == "Manuale") {
                if(orarioAccensione != nullptr) {
                    if(orarioSpegnimento == nullptr) {
                        ManualDevice *device = new ManualDevice(nome, std::stod(produzioneConsumo), *orarioAccensione.get());
                        active_devices.push_back(device);
                        all_devices.push_back(device);
                    } else {
                        ManualDevice *device = new ManualDevice(nome, std::stod(produzioneConsumo), *orarioAccensione.get(), *orarioSpegnimento.get());
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
                    CPDevice *device = new CPDevice(nome, std::stod(produzioneConsumo), *orarioAccensione.get());
                    active_devices.push_back(device);
                    all_devices.push_back(device);
                }
                orarioAccensione = std::make_shared<Time>(Time::fromString(durataAccensione));
                all_devices.push_back(new CPDevice(nome, std::stod(produzioneConsumo), *orarioAccensione.get()));
                orarioAccensione = nullptr;
            }
        }
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

/*void DomoticsSystem::checkSchedule() {
    for(auto it = active_devices.begin(); it != active_devices.end();) {
        Device *value = *it;
        ManualDevice *manualDevice = dynamic_cast<ManualDevice *>(value);
        CPDevice *cpDevice = dynamic_cast<CPDevice *>(value);
        if(manualDevice != nullptr) {
            if(*(value->get_start_time().get()) <= currentTime && *(manualDevice->get_stop_time().get()) > currentTime && value->is_on() == false) {
                value->switch_on(*(value->get_start_time().get()));
                powerLoad += value->KPower;
                log(*(value->get_start_time().get()), "Il dispostivo " + value->KName + " si é acceso");
                balancePower(value->KName);
            } else if(manualDevice->get_stop_time() != nullptr && (*(manualDevice->get_stop_time().get()) <= currentTime && value->is_on() == true || manualDevice->get_last_activation_time() != nullptr && currentTime < * (manualDevice->get_last_activation_time().get()) && value->is_on() == true)) {
                log(*(manualDevice->get_stop_time().get()), "Il dispostivo " + value->KName + " si é spento");
                value->switch_off(currentTime);
                powerLoad -= value->KPower;
            }
            ++it;
        } else if(cpDevice != nullptr) {
            if(value->get_last_activation_time() != nullptr && currentTime >= cpDevice->KDuration + * (value->get_last_activation_time().get()) && value->is_on() == true || value->get_start_time() != nullptr && currentTime < * (value->get_start_time().get()) && value->is_on() == true) {
                log(cpDevice->KDuration + * (value->get_last_activation_time().get()), "Il dispostivo " + value->KName + " si é spento");
                value->switch_off(cpDevice->KDuration + * (value->get_last_activation_time().get()));
                powerLoad -= value->KPower;
                //it = active_devices.erase(it);
            } else if(value->get_start_time() != nullptr && value->is_on() == false && *(value->get_start_time().get()) <= currentTime && currentTime < cpDevice->KDuration + * (value->get_start_time().get())) {
                value->switch_on(*(value->get_start_time().get()) );
                powerLoad += value->KPower;
                log(*(value->get_start_time().get()) , "Il dispostivo " + value->KName + " si é acceso");
                balancePower(value->KName);
                ++it;
            } else {
                ++it;
            }
        }
    }
}*/

void DomoticsSystem::checkSchedule() {
    for(auto it = active_devices.begin(); it != active_devices.end();) {
        Device *value = *it;
        ManualDevice *manualDevice = dynamic_cast<ManualDevice *>(value);
        CPDevice *cpDevice = dynamic_cast<CPDevice *>(value);
            if(cpDevice != nullptr && value->get_last_activation_time() != nullptr && currentTime >= cpDevice->KDuration + * (value->get_last_activation_time().get()) && value->is_on() == true || value->get_start_time() != nullptr && currentTime < * (value->get_start_time().get()) && value->is_on() == true) {
                log(cpDevice->KDuration + * (value->get_last_activation_time().get()), "Il dispostivo " + value->KName + " si é spento");
                value->switch_off(cpDevice->KDuration + * (value->get_last_activation_time().get()));
                powerLoad -= value->KPower;
            }else if(manualDevice != nullptr && manualDevice->get_stop_time() != nullptr && (*(manualDevice->get_stop_time().get()) <= currentTime && value->is_on() == true || manualDevice->get_last_activation_time() != nullptr && currentTime < * (manualDevice->get_last_activation_time().get()) && value->is_on() == true)) {
                log(*(manualDevice->get_stop_time().get()), "Il dispostivo " + value->KName + " si é spento");
                value->switch_off(currentTime);
                powerLoad -= value->KPower;
            }else if(manualDevice != nullptr && *(value->get_start_time().get()) <= currentTime && *(manualDevice->get_stop_time().get()) > currentTime && value->is_on() == false) {
                value->switch_on(*(value->get_start_time().get()));
                powerLoad += value->KPower;
                log(*(value->get_start_time().get()), "Il dispostivo " + value->KName + " si é acceso");
                balancePower(value->KName);
            } else if(cpDevice != nullptr && value->get_start_time() != nullptr && value->is_on() == false && *(value->get_start_time().get()) <= currentTime && currentTime < cpDevice->KDuration + * (value->get_start_time().get())) {
                value->switch_on(*(value->get_start_time().get()) );
                powerLoad += value->KPower;
                log(*(value->get_start_time().get()) , "Il dispostivo " + value->KName + " si é acceso");
                balancePower(value->KName);
            } 
            ++it;
    }
}

void DomoticsSystem::insertByTime(std::string device) {
    int index = getIndex(device, false);
    if (index == -1) {
        throw std::invalid_argument("device not found");
    }
    auto devicePtr = all_devices[index];
    auto it = std::lower_bound(active_devices.begin(), active_devices.end(), devicePtr, [](Device* a, Device* b) {
        return *a->get_start_time() < *b->get_start_time();
    });
    active_devices.insert(it, devicePtr);
}



void DomoticsSystem::setCurrentTime(const Time &newTime) {
    (newTime < currentTime) ? throw std::invalid_argument("time is lower than current time") : currentTime = newTime;
    //orderByStartTime();
    checkSchedule();
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
        all_devices[index]->switch_on(currentTime);
        log(currentTime, "Il dispostivo " + device + " si é acceso");
        if(auto cpDevice = dynamic_cast<CPDevice *>(active_devices.back())) {
            cpDevice->set_start_time(currentTime);
            dynamic_cast<CPDevice *>(all_devices[index])->set_start_time(currentTime);
        } else if(auto manualDevice = dynamic_cast<ManualDevice *>(active_devices.back())) {
            manualDevice->set_start_time(currentTime);
            dynamic_cast<ManualDevice *>(all_devices[index])->set_start_time(currentTime);
        }
        powerLoad += active_devices.back()->KPower;
        balancePower(device);
    } else if(activeIndex != -1 && !status) {
        active_devices[activeIndex]->switch_off(currentTime);
        powerLoad -= active_devices[activeIndex]->KPower;
         log(currentTime, "Il dispostivo " + device + " si é spento");
    } else {
        active_devices[activeIndex]->switch_on(currentTime);
        powerLoad += active_devices[activeIndex]->KPower;
         log(currentTime, "Il dispostivo " + device + " si é acceso");
        balancePower(device);
    }
}

void DomoticsSystem::balancePower(std::string last) {
    Time firstTime(23, 59), temp;
    std::string name;
    while(-powerLoad > KPowerLimit) {
        log(currentTime, "Il sistema è in sovraccarico energetico i gli ultimi dispositivi accesi verranno spenti");
        for(int i =0; i < active_devices.size() ;  i++) {
            temp = *active_devices[i]->get_start_time().get();
            if(active_devices[i]->KName != "Impianto fotovoltaico" && active_devices[i]->KName != "Frigorifero" && active_devices[i]->KName != last && firstTime >= temp && active_devices[i]->is_on()) {
                firstTime = temp;
                name = active_devices[i]->KName;
            }
        }
        int activeIndex = getIndex(name, true);
        int index = getIndex(name, false);
        active_devices[activeIndex]->switch_off(currentTime);
        log(currentTime, "Il dispostivo " + name + " si é spento");
        powerLoad -= active_devices[activeIndex]->KPower;
    }
}

void DomoticsSystem::setDeviceTime(const std::string &device, const Time &start, const Time &stop) {
    int activeIndex = getIndex(device, true);
    int index = getIndex(device, false);
    if(getIndex(device, false) == -1 || start > stop || start < currentTime || stop < currentTime) {
        throw std::invalid_argument("Error: invalid input data, please check the device name and the time");
    } else if(activeIndex == -1) {
        active_devices.push_back(all_devices[index]);
        if(dynamic_cast<CPDevice *>(active_devices.back()) != nullptr) {
            dynamic_cast<CPDevice *>(active_devices.back())->set_start_time(start);
            dynamic_cast<CPDevice *>(all_devices[index])->set_start_time(start);
        } else if(dynamic_cast<ManualDevice *>(active_devices.back()) != nullptr) {
            dynamic_cast<ManualDevice *>(active_devices.back())->set_new_timer(start, stop);
            dynamic_cast<ManualDevice *>(all_devices[index])->set_new_timer(start, stop);
        }
    } else if(*active_devices[activeIndex]->get_start_time().get() != start || *dynamic_cast<ManualDevice *>(active_devices[activeIndex])->get_stop_time().get() != stop) {
        if(dynamic_cast<CPDevice *>(active_devices[activeIndex]) != nullptr) {
            dynamic_cast<CPDevice *>(active_devices[activeIndex])->set_start_time(start);
            dynamic_cast<CPDevice *>(all_devices[index])->set_start_time(start);
        } else if(dynamic_cast<ManualDevice *>(active_devices[activeIndex]) != nullptr) {
            dynamic_cast<ManualDevice *>(active_devices[activeIndex])->set_new_timer(start, stop);
            dynamic_cast<ManualDevice *>(all_devices[index])->set_new_timer(start, stop);
        }
    }
    checkSchedule();
}

std::ostream &operator<<(std::ostream &out, const DomoticsSystem &sys) {
    for(auto value : sys.getDevices()) {
        if(auto cpDevice = dynamic_cast<CPDevice *>(value)) {
            out << *cpDevice << std::endl;
        } else if(auto manualDevice = dynamic_cast<ManualDevice *>(value)) {
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
    for(auto *value : active_devices) {
        value->resetDevice();
    }
    powerLoad = 0;
    checkSchedule();
}

void DomoticsSystem::resetTimers() {// to be finished
    for(auto *value : active_devices) {
        if(auto manualDevice = dynamic_cast<ManualDevice *>(value)) {
            manualDevice->removeTimer();
        } else {
            value->removeTimer();
        }
    }
    checkSchedule();
}
