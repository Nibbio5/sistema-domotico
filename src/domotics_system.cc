#include "../include/domotics_system.h"
#include <stdexcept>

DomoticsSystem::DomoticsSystem() : KPowerLimit{3.5}, currentTime{Time(0, 0)} {
    all_devices = setDevices();
    powerLoad = 0;
    startDevices();
}

std::vector<Device*> DomoticsSystem::setDevices()
{
    std::vector<Device*> dispositivi;
    std::ifstream file("./assets/devices.json");
    if(!file) {
        throw std::invalid_argument("il file devices.json non esiste");
    }
    std::string line;
    std::string nome;
    std::string durataAccensione;
    std::string produzioneConsumo;

    // manual parsing of the JSON (simple)
    while(std::getline(file, line)) {
        if(line.find("\"Nome\":") != std::string::npos) {
            nome = line.substr(line.find(":") + 2);
            nome = nome.substr(1, nome.size() - 3); // Remove the quotes and the final comma
        } else if(line.find("\"DurataAccensione\":") != std::string::npos) {
            durataAccensione = line.substr(line.find(":") + 2);

            durataAccensione = durataAccensione.substr(1, durataAccensione.size() - 3);
            // std::cout << durataAccensione << std::endl;
        } else if(line.find("\"ProduzioneConsumo\":") != std::string::npos) {
            produzioneConsumo = line.substr(line.find(":") + 2);
            produzioneConsumo = produzioneConsumo.substr(1, produzioneConsumo.size() - 3);

            // Create the device based on the durataAccensione
            if (durataAccensione == "Manuale")
            {
                if (nome == "Impianto fotovoltaico" || nome == "Frigorifero")
                {
                    active_devices.push_back(new ManualDevice(nome, std::stod(produzioneConsumo)));
                }
                else
                {
                    dispositivi.push_back(new ManualDevice(nome, std::stod(produzioneConsumo)));
                }
            }
            else
            {
                int ore, minuto;
                if (durataAccensione.substr(2).length() == 6)
                {
                    minuto = std::stoi(durataAccensione.substr(0, 2));
                    ore = 0;
                } else if(durataAccensione.length() > 15) {
                    ore = std::stoi(durataAccensione.substr(0, 2));
                    minuto = std::stoi(durataAccensione.substr(durataAccensione.rfind("e") + 1, 3));
                } else {
                    ore = std::stoi(durataAccensione.substr(0, 2));
                    minuto = 0;
                }
                dispositivi.push_back(new CPDevice(nome, std::stod(produzioneConsumo), Time(ore, minuto)));
            }
        }
    }
    return dispositivi;
}

std::vector<Device*> DomoticsSystem::getDevices() const
{
    return active_devices;
}

double DomoticsSystem::getPowerLoad() const {
    return powerLoad;
}

Time DomoticsSystem::getCurrentTime() const {
    return currentTime;
}

void DomoticsSystem::removeDevice(std::string device)
{
    if (dynamic_cast<ManualDevice *>(active_devices[getIndex(device)]) != nullptr)
    {
        powerLoad -= active_devices[getIndex(device)]->power;
        active_devices[getIndex(device)] = active_devices.back();
        active_devices.pop_back();
    }
}

int DomoticsSystem::getIndex(std::string device) const
{
    for (int i = 0; i < active_devices.size(); i++)
    {
        if (active_devices[i]->name == device)
        {
            return i;
        }
    }
    return -1;
}

void DomoticsSystem::startDevices()
{
    // accendo frigo e imposto fotovoltaico 8:00 alle 18:00
    dynamic_cast<ManualDevice *>(active_devices[getIndex("Impianto fotovoltaico")])->setNewTimer(Time(8, 0), Time(18, 0));
    dynamic_cast<ManualDevice *>(active_devices[getIndex("Frigorifero")])->setNewTimer(Time(0, 0), Time(23, 59));
}

void DomoticsSystem::currentMod()
{
    for (auto it = active_devices.begin(); it != active_devices.end();)
    {
        Device* value = *it;
        if (dynamic_cast<ManualDevice *>(value) != nullptr)
        {
            if (*(value->getStartTime().get()) < currentTime && *(dynamic_cast<ManualDevice *>(value)->getStopTime().get()) > currentTime && value->isDeviceOn() == false)
            {
                value->switchOn();
                powerLoad += value->power;
                dynamic_cast<ManualDevice *>(value)->setNewTimer(currentTime, Time(23, 59));
                std::cout << powerLoad << " turned on " << value->name << std::endl;
            }
            else if (*(dynamic_cast<ManualDevice *>(value)->getStopTime().get()) < currentTime && value->isDeviceOn() == true)
            {
                value->switchOff();
                powerLoad -= value->power;
                std::cout << powerLoad << " turned off " << value->name << std::endl;
            }
            ++it;
        }
        else if (currentTime - dynamic_cast<CPDevice *>(value)->duration >= *(value->getStartTime().get()) && value->isDeviceOn() == true)
        {
            value->switchOff();
            powerLoad -= value->power;
            std::cout << powerLoad << " turned off " << value->name << std::endl;
            it = active_devices.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void DomoticsSystem::setCurrentTime(const Time &newTime) {
    (newTime < currentTime) ? throw std::invalid_argument("time is lower than current time") : currentTime = newTime;
    currentMod();
}

void DomoticsSystem::changeDeviceStatus(bool status, std::string device)
{
    try
    {
        if (active_devices[getIndex(device)]->isDeviceOn() != status)
        {
            active_devices[getIndex(device)]->switchOff();
            powerLoad -= active_devices[getIndex(device)]->power;
            active_devices[getIndex(device)] = all_devices.back();
            all_devices.pop_back();
        }
    }
    catch(const std::exception& e)
    {
        if (status == true)
        {
             active_devices.push_back(all_devices[getIndex(device)]);
            active_devices[getIndex(device)]->switchOn();
            if (auto cpDevice = dynamic_cast<CPDevice *>(active_devices[getIndex(device)]))
            {
                cpDevice->setStartTime(currentTime);
            }
            else if (auto manualDevice = dynamic_cast<ManualDevice *>(active_devices[getIndex(device)]))
            {
                manualDevice->setNewTimer(currentTime, Time(23, 59));
            }
            powerLoad += active_devices[getIndex(device)]->power;
            balancePower(device);
        }
    }  
}

void DomoticsSystem::balancePower(std::string last)
{
    Time firstTime (23,59), temp;
    std::string name;
    while (-powerLoad > KPowerLimit)
    {
        for (auto *value : active_devices)
        {
            temp = *(value->getStartTime().get());
            if (value->name != "Solar Panel" && value->name != "Refrigerator" && firstTime > temp)
            {
                firstTime = temp;
                name = value->KName;
            }
        }
        if (*(active_devices[getIndex(last)]->getStartTime().get()) == firstTime)
        {
            name = last;
        }
        active_devices[getIndex(name)]->switchOff();
        powerLoad -= active_devices[getIndex(name)]->power;
    }
}

void DomoticsSystem::setDeviceTime(const std::string &device, const Time &start, const Time &stop)
{
    dynamic_cast<ManualDevice *>(all_devices[getIndex(device)])->setNewTimer(start, stop);;
    active_devices[getIndex(device)] = all_devices[getIndex(device)];
}

std::ostream &operator<<(std::ostream &out, const DomoticsSystem &sys)
{
    for (auto value : sys.getDevices())
    {
        if (auto cpDevice = dynamic_cast<CPDevice *>(value))
        {
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
    for(auto &[key, value] : active_devices) {
        deviceList.push_back(value);
    }
    return devices;
