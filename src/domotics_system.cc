#include "../include/domotics_system.h"

DomoticsSystem::DomoticsSystem() : KPowerLimit{3.5}
{
    all_devices = setDevices();
    Time currentTime = Time(0, 0);
    powerLoad = 0;
    startDevices();
}

std::map<std::string, Device *> DomoticsSystem::setDevices()
{
    std::map<std::string, Device *> dispositivi;
    std::ifstream file("./assets/devices.json");
    if (!file)
    {
        throw std::runtime_error("il file devices.json non esiste");
    }
    std::string line, nome, durataAccensione, produzioneConsumo;

    // manual parsing of the JSON (simple)
    while (std::getline(file, line))
    {
        if (line.find("\"Nome\":") != std::string::npos)
        {
            nome = line.substr(line.find(":") + 2);
            nome = nome.substr(1, nome.size() - 3); // Remove the quotes and the final comma
        }
        else if (line.find("\"DurataAccensione\":") != std::string::npos)
        {
            durataAccensione = line.substr(line.find(":") + 2);

            durataAccensione = durataAccensione.substr(1, durataAccensione.size() - 3);
            // std::cout << durataAccensione << std::endl;
        }
        else if (line.find("\"ProduzioneConsumo\":") != std::string::npos)
        {
            produzioneConsumo = line.substr(line.find(":") + 2);
            produzioneConsumo = produzioneConsumo.substr(1, produzioneConsumo.size() - 3);

            // Create the device based on the durataAccensione
            if (durataAccensione == "Manuale")
            {
                if (nome == "Impianto fotovoltaico" || nome == "Frigorifero")
                {
                    active_devices.insert(std::make_pair(nome, new ManualDevice(nome, std::stod(produzioneConsumo))));
                }
                else
                {
                    dispositivi.insert(std::make_pair(nome, new ManualDevice(nome, std::stod(produzioneConsumo))));
                }
            }
            else
            {
                int ore, minuto;
                if (durataAccensione.substr(2).length() == 6)
                {
                    minuto = std::stoi(durataAccensione.substr(0, 2));
                    ore = 0;
                }
                else if (durataAccensione.length() > 15)
                {
                    ore = std::stoi(durataAccensione.substr(0, 2));
                    minuto = std::stoi(durataAccensione.substr(durataAccensione.rfind("e") + 1, 3));
                }
                else
                {
                    ore = std::stoi(durataAccensione.substr(0, 2));
                    minuto = 0;
                }
                dispositivi.insert(std::make_pair(nome, new CPDevice(nome, std::stod(produzioneConsumo), Time(ore, minuto))));
            }
        }
    }
    return dispositivi;
}

std::map<std::string, Device *> DomoticsSystem::getDevices() const
{
    return active_devices;
}

double DomoticsSystem::getPowerLoad() const
{
    return powerLoad;
}

Time DomoticsSystem::getCurrentTime() const
{
    return currentTime;
}

void DomoticsSystem::removeDevice(std::string device)
{
    if (dynamic_cast<ManualDevice *>(active_devices[device]) != nullptr)
    {
        powerLoad -= active_devices[device]->power;
        active_devices.erase(device);
    }
}

void DomoticsSystem::startDevices()
{
    // accendo frigo e imposto fotovoltaico 8:00 alle 18:00
    dynamic_cast<ManualDevice *>(active_devices.at("Impianto fotovoltaico"))->setNewTimer(Time(8, 0), Time(18, 0));
    dynamic_cast<ManualDevice *>(active_devices.at("Frigorifero"))->setNewTimer(Time(0, 0), Time(23, 59));
}

void DomoticsSystem::currentMod()
{
    for (auto &[key, value] : active_devices)
    {
        if (dynamic_cast<ManualDevice *>(value) != nullptr)
        {
            if (*(value->getStartTime().get()) < currentTime && *(dynamic_cast<ManualDevice *>(value)->getStopTime().get()) > currentTime && value->isDeviceOn() == false)
            {
                value->switchOn();
                powerLoad += value->power;
                dynamic_cast<ManualDevice *>(value)->setNewTimer(currentTime, Time(23, 59));
                std::cout << powerLoad << " acceso " << value->name << std::endl;
            }
            else if (*(dynamic_cast<ManualDevice *>(value)->getStopTime().get()) < currentTime && value->isDeviceOn() == true)
            {
                value->switchOff();
                powerLoad -= value->power;
                std::cout << powerLoad << " spento " << value->name << std::endl;
            }
        }
    }
}

void DomoticsSystem::setCurrentTime(const Time &newTime)
{
    (newTime < currentTime) ? throw std::invalid_argument("time is lower than current time") : currentTime = newTime;
}

void DomoticsSystem::changeDeviceStatus(bool status, std::string device)
{
    if (active_devices.at(device)->isDeviceOn() != status)
    {
        if (status == true)
        {
            if (active_devices[device] == nullptr)
            {
                active_devices.insert(std::make_pair(device, all_devices.at(device)));
            }
            active_devices.at(device)->switchOn();
            if (auto cpDevice = dynamic_cast<CPDevice *>(active_devices[device]))
            {
                cpDevice->setStartTime(currentTime);
            }
            else if (auto manualDevice = dynamic_cast<ManualDevice *>(active_devices[device]))
            {
                manualDevice->setNewTimer(currentTime, Time(23, 59));
            }
            powerLoad += active_devices.at(device)->power;
        }
        else
        {
            active_devices.at(device)->switchOff();
            powerLoad -= active_devices.at(device)->power;
            active_devices.erase(device);
        }
    }
}

void DomoticsSystem::balancePower()
{
    Time firstTime (23,59), temp;
    std::string name;
    do
    {
        for (auto &[key, value] : active_devices)
        {
            temp = *(value->getStartTime().get());
            if (value->name != "Impianto fotovoltaico" && value->name != "Frigorifero" && firstTime > temp)
            {
                firstTime = temp;
                name = value->name;
            }
        }
        active_devices[name]->switchOff();
        powerLoad -= active_devices[name]->power;
    } while (powerLoad > KPowerLimit);
}

void DomoticsSystem::setDeviceTime(const std::string &device, const Time &start, const Time &stop)
{
    dynamic_cast<ManualDevice *>(all_devices.at(device))->setNewTimer(start, stop);;
    active_devices[device] = all_devices.at(device);
}

std::ostream &operator<<(std::ostream &out, const DomoticsSystem &sys)
{
    for (auto [key, value] : sys.getDevices())
    {
        if (auto cpDevice = dynamic_cast<CPDevice *>(value))
        {
            out << *cpDevice << std::endl;
        }
        else if (auto manualDevice = dynamic_cast<ManualDevice *>(value))
        {
            out << *manualDevice << std::endl;
        }
    }
    out << "Current time: " << sys.getCurrentTime() << std::endl;
    out << "Power load: " << sys.getPowerLoad() << std::endl;
    return out;
}
