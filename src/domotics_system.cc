#include "../include/domotics_system.h"

DomoticsSystem::DomoticsSystem() : KPowerLimit{3.5}, currentTime{Time(0, 0)}, powerLoad{0}
{
    all_devices = setDevices();
}

DomoticsSystem::DomoticsSystem(double power) : KPowerLimit{power}, currentTime{Time(0, 0)}, powerLoad{0}
{
    all_devices = setDevices();
}

std::vector<Device *> DomoticsSystem::setDevices()
{
    std::vector<Device *> dispositivi;
    std::ifstream file("./assets/devices.json");
    if (!file)
    {
        throw std::invalid_argument("il file devices.json non esiste");
    }
    std::string line;
    std::string nome;
    std::string durataAccensione;
    std::string produzioneConsumo;
    auto orarioAccensione = std::shared_ptr<Time>(nullptr);
    auto orarioSpegnimento = std::shared_ptr<Time>(nullptr);

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
        }
        else if(line.find("\"OrarioAccensione\":") != std::string::npos){
            orarioAccensione =  std::make_shared<Time>(Time::fromString(line.substr(line.find(":") + 2)));
        }
        else if(line.find("\"OrarioSpegnimento\":") != std::string::npos){
            orarioSpegnimento =  std::make_shared<Time>(Time::fromString(line.substr(line.find(":") + 2)));
        }
        else if (line.find("\"ProduzioneConsumo\":") != std::string::npos)
        {
            produzioneConsumo = line.substr(line.find(":") + 2);
            produzioneConsumo = produzioneConsumo.substr(1, produzioneConsumo.size() - 3);

            // Create the device based on the durataAccensione
            if (durataAccensione == "Manuale")
            {
                if (orarioAccensione != nullptr)
                {
                    (orarioSpegnimento == nullptr) ? active_devices.push_back(new ManualDevice(nome, std::stod(produzioneConsumo), Time(23,59))) :  
                                    active_devices.push_back(new ManualDevice(nome, std::stod(produzioneConsumo), *orarioAccensione.get(), *orarioSpegnimento.get()));
                    orarioAccensione = nullptr;
                    orarioSpegnimento = nullptr;
                }
                else
                {
                    dispositivi.push_back(new ManualDevice(nome, std::stod(produzioneConsumo)));
                }
            }
            else
            {
                int ore;
                int minuto;
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
                dispositivi.push_back(new CPDevice(nome, std::stod(produzioneConsumo), Time(ore, minuto)));
            }
        }
    }
    return dispositivi;
}

std::vector<Device *> DomoticsSystem::getDevices() const
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
    int index = getIndex(device, false);
    if(index == -1)
    {
        throw std::invalid_argument("device not found or already removed");
    }
    if (dynamic_cast<ManualDevice *>(active_devices[index]) != nullptr)
    {
        powerLoad -= active_devices[index]->power;
        active_devices[index] = active_devices.back();
        active_devices.pop_back();
    }else if (dynamic_cast<CPDevice *>(active_devices[index]) != nullptr)
    {
        powerLoad -= active_devices[index]->power;
        active_devices[index]->switchOff();
        active_devices[index] = active_devices.back();
        active_devices.pop_back();
    }
}

int DomoticsSystem::getIndex(std::string device, bool prova) const
{
    std::vector<Device *> devices;
    (!prova) ? devices = all_devices : devices = active_devices;
    for (int i = 0; i < devices.size(); i++)
    {
        if (devices[i]->name == device)
            return i;
    }
    return -1;
}

bool DomoticsSystem::isPresent(std::string device) const
{
        return getIndex(device, true) != -1;
}

void DomoticsSystem::checkSchedule()
{
    for (auto it = active_devices.begin(); it != active_devices.end();)
    {
        Device *value = *it;
        if (dynamic_cast<ManualDevice *>(value) != nullptr)
        {
            if (*(value->getStartTime().get()) < currentTime && *(dynamic_cast<ManualDevice *>(value)->getStopTime().get()) > currentTime && value->isDeviceOn() == false)
            {
                value->switchOn();
                powerLoad += value->power;
                dynamic_cast<ManualDevice *>(value)->setNewTimer(currentTime, Time(23, 59));
            }
            else if (*(dynamic_cast<ManualDevice *>(value)->getStopTime().get()) < currentTime && value->isDeviceOn() == true)
            {
                value->switchOff();
                powerLoad -= value->power;
            }
            ++it;
        }
        else if(dynamic_cast<CPDevice *>(value) != nullptr){
        if (currentTime >= dynamic_cast<CPDevice *>(value)->duration + *(value->getStartTime().get()) && value->isDeviceOn() == true)
        {
            value->switchOff();
            powerLoad -= value->power;
            it = active_devices.erase(it);
        }else if (value->isDeviceOn() == false && *(value->getStartTime().get()) < currentTime)
        {
            value->switchOn();
            powerLoad += value->power;
            balancePower(value->name);
            ++it;
        }
        else
        {
            ++it;
        }
        }
    }
}

void DomoticsSystem::setCurrentTime(const Time &newTime)
{
    (newTime < currentTime) ? throw std::invalid_argument("time is lower than current time") : currentTime = newTime;
    checkSchedule();
}

void DomoticsSystem::changeDeviceStatus(bool status, std::string device)
{
    if(getIndex(device, false) == -1)
    {
        throw std::invalid_argument("device not found");
    }
    int index = getIndex(device, true);
    if (index == -1 && status)
    {   
        active_devices.push_back(all_devices[getIndex(device, false)]);
        active_devices.back()->switchOn();
        if (auto cpDevice = dynamic_cast<CPDevice *>(active_devices.back()))
        {
            cpDevice->setStartTime(currentTime);
        }
        else if (auto manualDevice = dynamic_cast<ManualDevice *>(active_devices.back()))
        {
            manualDevice->setNewTimer(currentTime, Time(23, 59));
        }
        powerLoad += active_devices.back()->power;
        balancePower(device);
    } else if (index != -1 && !status)
    {
        active_devices[index]->switchOff();
        powerLoad -= active_devices[index]->power;
        active_devices[index] = active_devices.back();
        active_devices.pop_back();
    }
}

void DomoticsSystem::balancePower(std::string last)
{
    Time firstTime(23, 59), temp;
    std::string name;
    while (-powerLoad > KPowerLimit)
    {
        for (auto *value : active_devices)
        {
            temp = *(value->getStartTime().get());
            if (value->name != "Impianto fotovoltaico" && value->name != "Frigorifero" && firstTime > temp)
            {
                firstTime = temp;
                name = value->name;
            }
        }
        if (*(active_devices[active_devices.size()-2]->getStartTime().get()) == firstTime)
        {
            name = active_devices[active_devices.size()-2]->name;
        }
        int index = getIndex(name, true);
        active_devices[index]->switchOff();
        powerLoad -= active_devices[index]->power;
        active_devices[index] = active_devices.back();
        active_devices.pop_back();
    }
}

void DomoticsSystem::setDeviceTime(const std::string &device, const Time &start, const Time &stop) 
{
    if (getIndex(device, false) == -1 || start > stop || start < currentTime || stop < currentTime)  
    {
        throw std::invalid_argument("Error: invalid input data, please check the device name and the time");  
    }else if(getIndex(device, true) == -1)
    {
        
        int index = getIndex(device, false);
        active_devices.push_back(all_devices[index]);
        if(dynamic_cast<CPDevice *>(active_devices.back()) != nullptr) 
        {
            dynamic_cast<CPDevice *>(active_devices.back())->setStartTime(start);
        }else if(dynamic_cast<ManualDevice *>(active_devices.back()) != nullptr)
        {
            dynamic_cast<ManualDevice *>(active_devices.back())->setNewTimer(start, stop);
        }
    }
}

std::ostream &operator<<(std::ostream &out, const DomoticsSystem &sys)
{
    for (auto value : sys.getDevices())
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

std::vector<Device *> DomoticsSystem::getDevicesVector() const
{
    std::vector<Device *> deviceList;
    for (auto *value : active_devices)
    {
        deviceList.push_back(value);
    }
    return deviceList;
}