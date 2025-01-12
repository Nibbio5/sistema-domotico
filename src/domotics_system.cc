/**
 * @file domotics_system.h
 * @brief Header file for the DomoticsSystem class.
 *
 * This file contains the declaration of the DomoticsSystem class, which manages
 * a domotics system by controlling various devices. It includes methods for
 * managing device schedules, power load, and device statuses.
 *
 * @version 1.0
 * @date 2023-10-05
 *
 * @author Daniele Piccolo
 *
 * @note This file is part of the Domotics System project.
 */

#include "../include/domotics_system.h"

DomoticsSystem::DomoticsSystem() : KPowerLimit{3.5}, currentTime{Time(0, 0)}, powerLoad{0}, log{report::logs::getInstance()}
{
    setDevices();
    orderByStartTime();
    checkSchedule();
}

DomoticsSystem::DomoticsSystem(double power) : KPowerLimit{power}, currentTime{Time(0, 0)}, powerLoad{0}, log{report::logs::getInstance()}
{
    setDevices();
    orderByStartTime();
    checkSchedule();
}

void DomoticsSystem::setDevices()
{
    std::ifstream file("./assets/devices.json");
    if (!file)
    {
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
        else if (line.find("\"OrarioAccensione\":") != std::string::npos)
        {
            orarioAccensione = std::make_shared<Time>(Time::fromString(line.substr(line.find(":") + 2)));
        }
        else if (line.find("\"OrarioSpegnimento\":") != std::string::npos)
        {
            orarioSpegnimento = std::make_shared<Time>(Time::fromString(line.substr(line.find(":") + 2)));
        }
        else if (line.find("\"WhiteList\":") != std::string::npos)
        {
            isOnWhiteList = line.find("true") != std::string::npos;
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
                    if (orarioSpegnimento == nullptr)
                    {
                        ManualDevice *device = new ManualDevice(nome, std::stod(produzioneConsumo), *orarioAccensione.get(), isOnWhiteList);
                        active_devices.push_back(device);
                        all_devices.push_back(device);
                    }
                    else
                    {
                        ManualDevice *device = new ManualDevice(nome, std::stod(produzioneConsumo), *orarioAccensione.get(), *orarioSpegnimento.get(), isOnWhiteList);
                        active_devices.push_back(device);
                        all_devices.push_back(device);
                    }
                    orarioAccensione = nullptr;
                    orarioSpegnimento = nullptr;
                    isOnWhiteList = false;
                }
                else
                {
                    all_devices.push_back(new ManualDevice(nome, std::stod(produzioneConsumo)));
                }
            }
            else
            {
                if (orarioAccensione != nullptr)
                {
                    CPDevice *device = new CPDevice(nome, std::stod(produzioneConsumo), *orarioAccensione.get(), isOnWhiteList);
                    active_devices.push_back(device);
                    all_devices.push_back(device);
                }
                orarioAccensione = std::make_shared<Time>(Time::fromString(durataAccensione));
                all_devices.push_back(new CPDevice(nome, std::stod(produzioneConsumo), *orarioAccensione.get(), isOnWhiteList));
                orarioAccensione = nullptr;
                 isOnWhiteList = false;
            }
        }
    }
}

std::vector<Device *> DomoticsSystem::getDevices() const
{
    return all_devices;
}

double DomoticsSystem::getPowerLoad() const
{
    return powerLoad;
}

Time DomoticsSystem::getCurrentTime() const
{
    return currentTime;
}

void DomoticsSystem::removeDeviceTimer(std::string device)
{
    int index = getIndex(device, false);
    int activeIndex = getIndex(device, true);
    if (index == -1)
    {
        throw std::invalid_argument("device not found or already removed");
    }
    if (dynamic_cast<ManualDevice *>(active_devices[activeIndex]) != nullptr)
    {
        dynamic_cast<ManualDevice *>(active_devices[activeIndex])->removeTimer();
        dynamic_cast<ManualDevice *>(all_devices[index])->removeTimer();
    }
    else if (dynamic_cast<CPDevice *>(active_devices[activeIndex]) != nullptr)
    {
        dynamic_cast<CPDevice *>(active_devices[activeIndex])->removeTimer();
        dynamic_cast<CPDevice *>(all_devices[index])->removeTimer();
    }
    checkSchedule();
}

int DomoticsSystem::getIndex(std::string device, bool prova) const
{
    std::vector<Device *> devices;
    (!prova) ? devices = all_devices : devices = active_devices;
    for (int i = 0; i < devices.size(); i++)
    {
        if (devices[i]->KName == device)
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
    Time tempo = Time(0, 0);
    for (size_t i = 0; i <= active_devices.size();)
    {
        if (i < active_devices.size())
        {
            if (*active_devices[i]->get_start_time() <= currentTime)
                tempo = *active_devices[i]->get_start_time();
        }
        else
        {
            tempo = currentTime;
        }

        bool deviceRemoved = false;
        for (auto it = active_devices.begin(); it != active_devices.end();)
        {
            Device *device = *it;
            std::shared_ptr<const Time> startTime = device->get_start_time();
            if (ManualDevice *manualDevice = dynamic_cast<ManualDevice *>(device))
            {
                std::shared_ptr<const Time> stopTime = manualDevice->get_stop_time();

                if (checkScheduleManualDevice(manualDevice, tempo, startTime.get(), stopTime.get()))
                {
                    it = active_devices.erase(it);
                    deviceRemoved = true;
                    continue;
                }
            }
            else if (CPDevice *cpDevice = dynamic_cast<CPDevice *>(device))
            {
                Time duration = cpDevice->KDuration;
                if(checkScheduleCpDevice(cpDevice, tempo, startTime.get())) {
                     it = active_devices.erase(it);
                    deviceRemoved = true;
                    continue;
                }
            }
            ++it;
        }
        if (!deviceRemoved)
        {
            ++i;
        }
    }
}

bool DomoticsSystem::checkScheduleManualDevice(ManualDevice *manualDevice, const Time nowTime, const Time *startTime, const Time *stopTime)
{

    if (startTime && *startTime <= nowTime && stopTime && *stopTime > nowTime && !manualDevice->is_on()) // check if start time is before current time and stop time is after current time and the device is off

    {
        manualDevice->switch_on(*startTime);
        powerLoad += manualDevice->KPower;

        //log.addLog(report::message(*startTime, "Il dispositivo " + manualDevice->KName + " si é acceso")); // log message when the device is turned on
        std::cout << "Il dispositivo " << manualDevice->KName << " si é acceso" << std::endl;

        balancePower(manualDevice->KName); // balance power when the device is turned on
    }
    else if (stopTime && *stopTime <= nowTime && manualDevice->is_on()) // check if stop time is not a nullpointer and that is before current time and the device is on
    {
        //log.addLog(report::message(*stopTime, "Il dispositivo " + manualDevice->KName + " si é spento")); // log message when the device is turned off
        std::cout << "Il dispositivo " << manualDevice->KName << " si é spento" << std::endl;

        manualDevice->switch_off(*stopTime);
        powerLoad -= manualDevice->KPower;

        return true; // return true when the device is turned off, in order to remove it from the active devices
    }
    return false; // return false when the device started or is still on
}

bool DomoticsSystem::checkScheduleCpDevice(CPDevice *cpDevice, const Time nowTime, const Time *startTime)
   { 
    Time duration = cpDevice->KDuration;
    if (startTime && !cpDevice->is_on() && *startTime <= nowTime && *startTime + duration >= nowTime)
    {
                    cpDevice->switch_on(*startTime);
                    powerLoad += cpDevice->KPower;
                    //log.addLog(report::message(*startTime, "Il dispositivo " + cpDevice->KName + " si é acceso"));
                    std::cout << "Il dispositivo " << cpDevice->KName << " si é acceso" << std::endl;
                    balancePower(cpDevice->KName);
     }
     if (startTime && nowTime >= *startTime + duration && cpDevice->is_on())
    {
                    //log.addLog(report::message(*startTime + duration, "Il dispositivo " + cpDevice->KName + " si é spento"));
                    std::cout << "Il dispositivo " << cpDevice->KName << " si é spento" << std::endl;
                    cpDevice->switch_off(*startTime + duration);
                    powerLoad -= cpDevice->KPower;
                    return true;
     }
    return false;
}

void DomoticsSystem::orderByStartTime()
{
    std::sort(active_devices.begin(), active_devices.end(), [](Device *a, Device *b)
              {
        auto startTimeA = a->get_start_time();
        auto startTimeB = b->get_start_time();
        //if (!startTimeA && !startTimeB) return false;
        //if (!startTimeA) return false;
        //if (!startTimeB) return true;
        return *startTimeA < *startTimeB; });
}

void DomoticsSystem::setCurrentTime(const Time &newTime)
{
    (newTime < currentTime) ? throw std::invalid_argument("time is lower than current time") : currentTime = newTime;
    orderByStartTime();
    checkSchedule();
    log.addLog(report::message(currentTime, "L'orario attuale è " + currentTime.getHourString() + ":" + currentTime.getMinuteString()));
}

void DomoticsSystem::changeDeviceStatus(bool status, std::string device)
{
    int index = getIndex(device, false);
    if (index == -1)
    {
        throw std::invalid_argument("device not found");
    }
    int activeIndex = getIndex(device, true);
    if (activeIndex == -1 && status)
    {
        active_devices.push_back(all_devices[index]);
        active_devices.back()->switch_on(currentTime);
        active_devices.back()->set_start_time(currentTime);
        log.addLog(report::message(currentTime, "Il dispositivo " + device + " si é acceso"));
        powerLoad += active_devices.back()->KPower;
        balancePower(device);
    }
    else if (activeIndex != -1 && !status)
    {
        log.addLog(report::message(currentTime, "Il dispositivo " + device + " si é spento"));
        active_devices[activeIndex]->switch_off(currentTime);
        powerLoad -= active_devices[activeIndex]->KPower;
        active_devices.erase(active_devices.begin() + activeIndex);
    }
    else
    {
        active_devices[activeIndex]->switch_on(currentTime);
        active_devices[activeIndex]->set_start_time(currentTime);
        powerLoad += active_devices[activeIndex]->KPower;
        log.addLog(report::message(currentTime, "Il dispositivo " + device + " si é acceso"));
        balancePower(device);
    }
}

void DomoticsSystem::balancePower(std::string last)
{
    Time latestTime(0, 0);
    std::string name;
    while (-powerLoad > KPowerLimit)
    {
        //log.addLog(report::message(currentTime, "Il sistema è in sovraccarico energetico, gli ultimi dispositivi accesi verranno spenti"));
        std::cout << "Il sistema è in sovraccarico energetico, gli ultimi dispositivi accesi verranno spenti" << std::endl;
        for (int i = 0; i < active_devices.size(); i++)
        {
            if (active_devices[i]->get_start_time() != nullptr &&
                !active_devices[i]->KIsOnWhiteList &&
                *active_devices[i]->get_start_time() >= latestTime &&
                active_devices[i]->KName != last &&
                active_devices[i]->is_on())
            {
                latestTime = *active_devices[i]->get_start_time();
                name = active_devices[i]->KName;
            }
        }
        int activeIndex = getIndex(name, true);
        if (activeIndex != -1)
        {
            //log.addLog(report::message(currentTime, "Il dispositivo " + name + " si é spento"));
            std::cout << "Il dispositivo " << name << " si é spento" << std::endl;
            active_devices[activeIndex]->switch_off(currentTime);
            powerLoad -= active_devices[activeIndex]->KPower;
            active_devices.erase(active_devices.begin() + activeIndex);
        }
        latestTime = Time(0, 0); // Reset latestTime for the next iteration
    }
}
void DomoticsSystem::setDeviceTime(const std::string &device, const Time &start, const Time &stop)
{
    int index = getIndex(device, false);
    if (index == -1 || start > stop || start < currentTime || stop <= currentTime)
    {
        throw std::invalid_argument("Invalid input data, please check the device name and the time");
    }else if (start == currentTime && stop > currentTime){
         throw std::invalid_argument("usa il comando 'set ${DEVICENAME} on' per accendere il dispositivo al momento attuale");
    }
    {
        changeDeviceStatus(true, device);
        return;
    }

    int activeIndex = getIndex(device, true);
    Device *targetDevice = all_devices[index];

    if (activeIndex == -1)
    {
        active_devices.push_back(targetDevice);
    }
    else
    {
        targetDevice = active_devices[activeIndex];
    }

    if (CPDevice *cpDevice = dynamic_cast<CPDevice *>(targetDevice))
    {
        cpDevice->set_start_time(start);
    }
    else if (ManualDevice *manualDevice = dynamic_cast<ManualDevice *>(targetDevice))
    {
        manualDevice->set_new_timer(start, stop);
    }
    orderByStartTime();
    checkSchedule();
}

std::ostream &operator<<(std::ostream &out, const DomoticsSystem &sys)
{
    for (auto value : sys.getDevices())
    {
        if (CPDevice *cpDevice = dynamic_cast<CPDevice *>(value))
        {
            out << *cpDevice << std::endl;
        }
        else if (ManualDevice *manualDevice = dynamic_cast<ManualDevice *>(value))
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

void DomoticsSystem::resetTime()
{
    currentTime = Time(0, 0);
    for (auto *value : all_devices)
    {
        if (value->get_start_time() != nullptr && getIndex(value->KName, true) == -1)
        {
            active_devices.push_back(value);
        }
        value->resetDevice();
    }
    orderByStartTime();
    powerLoad = 0;
    checkSchedule();
}

void DomoticsSystem::resetTimers()
{
    // to be finished
    for (auto *value : active_devices)
    {
        if (ManualDevice *manualDevice = dynamic_cast<ManualDevice *>(value))
        {
            manualDevice->removeTimer();
        }
        else
        {
            value->removeTimer();
        }
    }
    checkSchedule();
}

void DomoticsSystem::resetAll()
{
    currentTime = Time(0, 0);
    for (auto *value : all_devices)
    {
        value->resetDeviceAndTimer();
    }
    active_devices.clear();
    powerLoad = 0;
    checkSchedule();
}
