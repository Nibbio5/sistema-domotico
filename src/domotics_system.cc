/**
 * @file domotics_system.cc
 * @brief cc file for the DomoticsSystem class.
 *
 * This file contains the definitions of the DomoticsSystem class, which manages
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

DomoticsSystem::DomoticsSystem(const double &power) : KPowerLimit{power}, currentTime{Time(0, 0)}, powerLoad{0}, log{report::logs::getInstance()}
{
    setDevices();
    orderByStartTime();
    checkSchedule();
}

DomoticsSystem::~DomoticsSystem()
{
    for (auto *value : all_devices)
    {
        delete value;
    }
    switchedDevices.clear();
    all_devices.clear();
    active_devices.clear();
}

void DomoticsSystem::setDevices()
{
    std::ifstream file("./assets/devices.json");
    if (!file)
    {
        throw std::invalid_argument("Il file devices.json non esiste");
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
            durataAccensione = line.substr(line.find(":") + 2);                         // Get the value of the key
            durataAccensione = durataAccensione.substr(1, durataAccensione.size() - 3); // Remove the quotes and the final comma
        }
        else if (line.find("\"OrarioAccensione\":") != std::string::npos)
        {                                                                                                 // Check if the line contains the key "OrarioAccensione"
            orarioAccensione = std::make_shared<Time>(Time::fromString(line.substr(line.find(":") + 2))); // Create a new Time object from the value of the key
        }
        else if (line.find("\"OrarioSpegnimento\":") != std::string::npos)
        {                                                                                                  // Check if the line contains the key "OrarioSpegnimento"
            orarioSpegnimento = std::make_shared<Time>(Time::fromString(line.substr(line.find(":") + 2))); // Create a new Time object from the value of the key
        }
        else if (line.find("\"WhiteList\":") != std::string::npos)
        {                                                           // Check if the line contains the key "WhiteList"
            isOnWhiteList = line.find("true") != std::string::npos; // Check if the value is "true"
        }
        else if (line.find("\"ProduzioneConsumo\":") != std::string::npos)
        { // Check if the line contains the key "ProduzioneConsumo"
            produzioneConsumo = line.substr(line.find(":") + 2);
            produzioneConsumo = produzioneConsumo.substr(1, produzioneConsumo.size() - 3); // Remove the quotes and the final comma
            // Create the device based on the durataAccensione
            if (durataAccensione == "Manuale")
            { // Check if the device is a ManualDevice
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
                {                                                                                                                // Check if the device is a CPDevice
                    CPDevice *device = new CPDevice(nome, std::stod(produzioneConsumo), *orarioAccensione.get(), isOnWhiteList); // Create a new CPDevice object
                    active_devices.push_back(device);                                                                            // Add the device to the active devices
                    all_devices.push_back(device);                                                                               // Add the device to the all devices
                }
                orarioAccensione = std::make_shared<Time>(Time::fromString(durataAccensione));
                all_devices.push_back(new CPDevice(nome, std::stod(produzioneConsumo), *orarioAccensione.get(), isOnWhiteList));
                orarioAccensione = nullptr;
                isOnWhiteList = false; // Reset the flag
            }
        }
    }
}

std::vector<Device *> DomoticsSystem::getDevices() const
{
    return all_devices;
}

const double &DomoticsSystem::getPowerLoad() const
{
    return powerLoad;
}

const Time &DomoticsSystem::getCurrentTime() const
{
    return currentTime;
}

void DomoticsSystem::removeDeviceTimer(std::string device)
{
    int index = getIndex(device, false);      // get the index of the device in the all devices
    int activeIndex = getIndex(device, true); // get the index of the device in the active devices

    if ((activeIndex == -1 || index == -1) || index == -1 // check if the device is not in the all_devices or in the active_devices and if the device is not in switchedDevices
        || switchedDevices.end() != std::find(switchedDevices.begin(), switchedDevices.end(), active_devices[activeIndex]))
    {
        throw std::invalid_argument("Dispositivo non trovato o senza timer");
    }
    auto manualDevice = dynamic_cast<ManualDevice *>(all_devices[index]);
    auto cpDevice = dynamic_cast<CPDevice *>(all_devices[index]);
    if (activeIndex != -1)
    { // check if the device is in the active devices

        if (!active_devices[activeIndex]->is_on())
        { // check if the device is off

            if (manualDevice != nullptr)
            {

                log.addLog(report::message(currentTime, "Rimosso il timer dal dispositivo " + manualDevice->KName));
                manualDevice->removeTimer();
                active_devices.erase(active_devices.begin() + activeIndex);
            }
            else
            {

                log.addLog(report::message(currentTime, "Rimosso il timer dal dispositivo " + cpDevice->KName));
                cpDevice->removeTimer();
                active_devices.erase(active_devices.begin() + activeIndex);
            }
        }
        else if (manualDevice != nullptr)
        { // check if the device is active is a ManualDevice

            log.addLog(report::message(currentTime, "Rimosso il timer dal dispositivo " + manualDevice->KName));
            manualDevice->removeStopTime();
            switchedDevices.push_back(manualDevice);
        }
        else
        { // if is not a manual device is a CPDevice

            log.addLog(report::message(currentTime, "Rimosso il timer dal dispositivo " + cpDevice->KName));
            switchedDevices.push_back(cpDevice);
        }
    }
    checkSchedule();
}

int DomoticsSystem::getIndex(const std::string &device, bool isActive) const
{
    const std::vector<Device *> &devices = isActive ? active_devices : all_devices; // set the vector to search in
    for (size_t i = 0; i < devices.size(); ++i)
    {
        if (devices[i]->KName == device)
        {
            return static_cast<int>(i);
        }
    }
    return -1; // return -1 if the device is not found
}

void DomoticsSystem::checkSchedule()
{
    Time nowTime = Time(0, 0);
    int i = 0, size = 0;
    do
    { // iterate over the active devices
        if (i < active_devices.size())
        {                                                            // check if the index is in the range of the active devices
            if (*active_devices[i]->get_start_time() <= currentTime) // check if the start time of the device is before or equals the current time
                nowTime = *active_devices[i]->get_start_time();
        }
        else
            nowTime = currentTime; // else set the tempo to the current time

        size = active_devices.size(); // get the size of the active devices
        bool deviceRemoved = false;
        for (auto it = active_devices.begin(); it != active_devices.end() && active_devices.size() != 0;)
        { // iterate over the active devices
            Device *device = *it;
            std::shared_ptr<const Time> startTime = device->get_start_time(); // get the start time of the device
            if (ManualDevice *manualDevice = dynamic_cast<ManualDevice *>(device))
            { // check if the device is a ManualDevice
                std::shared_ptr<const Time> stopTime = manualDevice->get_stop_time();

                if (checkScheduleManualDevice(manualDevice, nowTime, startTime.get(), stopTime.get()))
                {                                  // verify if the device is turned off
                    it = active_devices.erase(it); // remove the device from the active devices
                    continue;
                }
            }
            else if (CPDevice *cpDevice = dynamic_cast<CPDevice *>(device))
            { // check if the device is a CPDevice
                Time duration = cpDevice->KDuration;
                if (checkScheduleCpDevice(cpDevice, nowTime, startTime.get()))
                {                                  // verify if the device is turned off
                    it = active_devices.erase(it); // remove the device from the active devices
                    if(cpDevice->get_start_time() != nullptr && *cpDevice->get_start_time().get() > nowTime)
                    {
                    active_devices.push_back(device);
                    }
                    continue;
                }
            }
            ++it;
        }
        if (size != active_devices.size())
        { // check if the size of the active devices is different from the previous size (a device or more are/is removed)
            i -= size - active_devices.size();
        }
        else
            ++i; // increment i if the device is not removed

    } while (nowTime < currentTime);
}

bool DomoticsSystem::checkScheduleManualDevice(ManualDevice *manualDevice, const Time nowTime, const Time *startTime, const Time *stopTime)
{

    if (startTime && *startTime <= nowTime && stopTime && *stopTime > nowTime && !manualDevice->is_on())
    { // check if start time is before current time and stop time is after current time and the device is off
        manualDevice->switch_on(*startTime);
        powerLoad += manualDevice->KPower;

        log.addLog(report::message(*startTime, "Il dispositivo " + manualDevice->KName + " si é acceso")); // log message when the device is turned on

        balancePower(manualDevice->KName, nowTime); // balance power when the device is turned on
    }
    if (stopTime && *stopTime <= nowTime && manualDevice->is_on())
    {                                                                                                     // check if stop time is not a nullpointer and that is before current time and the device is on
        log.addLog(report::message(*stopTime, "Il dispositivo " + manualDevice->KName + " si é spento")); // log message when the device is turned off
        manualDevice->switch_off(*stopTime);
        powerLoad -= manualDevice->KPower;

        return true; // return true when the device is turned off, in order to remove it from the active devices
    }
    return false; // return false when the device started or is still on
}

bool DomoticsSystem::checkScheduleCpDevice(CPDevice *cpDevice, const Time nowTime, const Time *startTime)
{
    bool deviceRemoved = false;
    Time duration = cpDevice->KDuration;
    std::shared_ptr<const Time> lastActivationTime = cpDevice->get_last_activation_time();
    if ((startTime && nowTime >= *startTime + duration && cpDevice->is_on()) 
    || (lastActivationTime && nowTime >= *lastActivationTime + duration && cpDevice->is_on()))
    {
        if (lastActivationTime) // check if the starttime/lastActivationTime is before the nowTime and the device is on
        {
            log.addLog(report::message(*lastActivationTime + duration, "Il dispositivo " + cpDevice->KName + " si é spento"));
            cpDevice->switch_off(*lastActivationTime + duration);
        }
        else
        {
            log.addLog(report::message(*lastActivationTime + duration, "Il dispositivo " + cpDevice->KName + " si é spento"));
            cpDevice->switch_off(*startTime + duration);
        }
        powerLoad -= cpDevice->KPower;
        deviceRemoved = true; // return true when the device is turned off, in order to remove it from the active devices
    }

    if (startTime && !cpDevice->is_on() && *startTime <= nowTime && *startTime + duration > nowTime)
    { // check if the device is off and the start time is before the current time and the start time + duration is after the current time
        cpDevice->switch_on(*startTime);
        powerLoad += cpDevice->KPower;
        log.addLog(report::message(*startTime, "Il dispositivo " + cpDevice->KName + " si é acceso"));
        balancePower(cpDevice->KName, nowTime); // balance power when the device is turned on
        deviceRemoved = false;                  // return false when the device is turned on
    }
    return deviceRemoved; // return false when the device started or is still on
}

void DomoticsSystem::orderByStartTime()
{ // sort the active devices by start time, the first device is the one with the earliest start time
    std::sort(active_devices.begin(), active_devices.end(), [](Device *a, Device *b)
              {
                  return *a->get_start_time() < *b->get_start_time(); // compare the start time of the devices
              });
}

void DomoticsSystem::setCurrentTime(const Time &newTime)
{
    if (newTime < currentTime)
    {
        throw std::invalid_argument("Il tempo fornito è minore del tempo corrente");
    }
    else
        currentTime = newTime;

    orderByStartTime();
    if (newTime == Time(23, 59))
    {             // check if the new time is the end of the day
        endDay(); // turn off all devices
        log.addLog(report::message(currentTime, "Il giorno è finito, tutti i dispositivi sono stati spenti"));
    }
    else
    {
        checkSchedule(); // check the schedule of the devices
        log.addLog(report::message(currentTime, "L'orario attuale è " + currentTime.getHourString() + ":" + currentTime.getMinuteString()));
    }
}

void DomoticsSystem::changeDeviceStatus(bool status, const std::string &device)
{
    int index = getIndex(device, false);
    if (index == -1)
    { // check if the device is not in the all_devices
        throw std::invalid_argument("Dispositivo non trovato");
    }
    int activeIndex = getIndex(device, true); // get the index of the device in the active devices

    if (status)
    { // if the status is true (turn on the device)
        if (activeIndex == -1)
        {                                                  // if the device is not already active
            active_devices.push_back(all_devices[index]);  // add the device to the active devices
            switchedDevices.push_back(all_devices[index]); // add the device to the switched devices
            active_devices.back()->switch_on(currentTime);
            active_devices.back()->set_start_time(currentTime);
            log.addLog(report::message(currentTime, "Il dispositivo " + device + " si è acceso"));
            powerLoad += active_devices.back()->KPower;
            balancePower(device, currentTime);
        }
        else
        {
            throw std::invalid_argument("Il dispositivo è già attivo");
        }
    }
    else
    { // else turn off the device
        if (activeIndex != -1)
        { // if the device is in the active devices
            log.addLog(report::message(currentTime, "Il dispositivo " + device + " si è spento"));
            active_devices[activeIndex]->switch_off(currentTime);
            powerLoad -= active_devices[activeIndex]->KPower;
            active_devices.erase(active_devices.begin() + activeIndex); // remove the device from the active devices
        }
        else
        {
            throw std::invalid_argument("Il dispositivo è già spento");
        }
    }
    orderByStartTime(); // order the devices by start time
}

void DomoticsSystem::balancePower(const std::string &last, const Time &nowTime)
{
    Time latestTime(0, 0);
    std::string name;
    bool onlyWhiteListDevices = true;

    if (KPowerLimit == 0 && powerLoad < 0)
    { // check if the power limit is 0 and the power load is negative
        log.addLog(report::message(nowTime, "Il sistema è in sovraccarico energetico, gli ultimi dispositivi accesi verranno spenti"));
        Device *device = active_devices[getIndex(last, true)];
        device->switch_off(nowTime);
        log.addLog(report::message(nowTime, "Il dispositivo " + last + " si é spento"));
        powerLoad -= device->KPower;
        active_devices.erase(active_devices.begin() + getIndex(last, true));
    }

    while (-powerLoad > KPowerLimit)
    { // while the power load is greater than the power limit
        log.addLog(report::message(nowTime, "Il sistema è in sovraccarico energetico, gli ultimi dispositivi accesi verranno spenti"));

        for (int i = 0; i < active_devices.size(); i++)
        {
            // Check if the device is on, if the start time is after the latest time, if the device is not the last device and if the device is not in the white list
            if (active_devices[i]->get_start_time() != nullptr && *active_devices[i]->get_start_time() >= latestTime && active_devices[i]->KName != last && active_devices[i]->is_on())
            {
                if (!active_devices[i]->KIsOnWhiteList)
                    onlyWhiteListDevices = false; // check if the device is not in the white list
                latestTime = *active_devices[i]->get_start_time();
                name = active_devices[i]->KName;
            }
        }

        if (onlyWhiteListDevices)
        { // check if all the devices are in the white list
            auto it = std::find_if(active_devices.begin(), active_devices.end(), [&](Device *device)
                                   {
                                       return device->KName == last && device->KIsOnWhiteList; // check if the device is the last device and is in the white list
                                   });
            if (it != active_devices.end())
            { // if the last is not in the white list
                int activeIndex = getIndex(name, true);
                if (activeIndex != -1)
                {
                    log.addLog(report::message(nowTime, "Il dispositivo " + name + " si é spento"));
                    active_devices[activeIndex]->switch_off(nowTime);
                    powerLoad -= active_devices[activeIndex]->KPower;
                    active_devices.erase(active_devices.begin() + activeIndex);
                }
            }
            else
            { // else turn off the last device
                int lastIndex = getIndex(last, true);
                log.addLog(report::message(nowTime, "Tutti i dispositivi attivi sono nella white list, il dispositivo " + last + " verrà spento dato che non è nella white list"));
                active_devices[lastIndex]->switch_off(nowTime);
                powerLoad -= active_devices[lastIndex]->KPower;
                active_devices.erase(active_devices.begin() + lastIndex);
            }
        }
        else
        { // else turn off the the non whitelist device before last
            int activeIndex = getIndex(name, true);
            if (activeIndex != -1)
            {
                log.addLog(report::message(nowTime, "Il dispositivo " + name + " si é spento"));
                active_devices[activeIndex]->switch_off(nowTime);
                powerLoad -= active_devices[activeIndex]->KPower;
                active_devices.erase(active_devices.begin() + activeIndex);
            }
        }

        latestTime = Time(0, 0); // Reset latestTime for the next iteration
    }
}

void DomoticsSystem::setDeviceTime(const std::string &device, const Time &start, const Time &stop)
{
    int index = getIndex(device, false);
    if (index == -1 || start > stop || start < currentTime || stop <= currentTime)
    {
        // check if the device is not in the all_devices or the start time is after the stop time or the start time is before the current time or the stop time is before or equals the current time
        throw std::invalid_argument("Dispositivo inesistente o tempo errato");
    }
    else if (start == currentTime && stop > currentTime)
    { // check if the start time is the current time and the stop time is after the current time
        throw std::invalid_argument("Usa il comando 'set \"${DEVICENAME}\" on' per accendere il dispositivo al momento attuale");
    }
    int activeIndex = getIndex(device, true);
    Device *targetDevice = all_devices[index];

    if (activeIndex == -1)
    { // if the device is not in the active devices add it
        active_devices.push_back(targetDevice);
    }
    else
    { // else remove the device from the switched devices
        switchedDevices.erase(std::remove(switchedDevices.begin(), switchedDevices.end(), targetDevice), switchedDevices.end());
    }

    if (CPDevice *cpDevice = dynamic_cast<CPDevice *>(targetDevice))
    {
        cpDevice->set_start_time(start);
        log.addLog(report::message(currentTime, "Impostato un time per il dispositivo " + cpDevice->KName + " dalle " + start.getHourString() + ":" + start.getMinuteString() + " alle " + (start + cpDevice->KDuration).getHourString() + ":" + (start + cpDevice->KDuration).getMinuteString()));
    }
    else if (ManualDevice *manualDevice = dynamic_cast<ManualDevice *>(targetDevice))
    {
        manualDevice->set_new_timer(start, stop);
        log.addLog(report::message(currentTime, "Impostato un time per il dispositivo " + manualDevice->KName + " dalle " + start.getHourString() + ":" + start.getMinuteString() + " alle " + stop.getHourString() + ":" + stop.getMinuteString()));
    }
    orderByStartTime();
    checkSchedule();
}

std::ostream &operator<<(std::ostream &out, const DomoticsSystem &sys)
{ // overload the << operator to output the system status not used in the project
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

void DomoticsSystem::resetTime()
{
    for (auto *value : switchedDevices)
    { // reset all the switched devices timers
        value->resetDeviceAndTimer();
    }

    switchedDevices.clear();
    active_devices.clear();

    for (auto *value : all_devices)
    { // repopulate the active devices with the devices that have a start time or timer
        // value->switch_off(currentTime);
        if (value->get_start_time() != nullptr && getIndex(value->KName, true) == -1)
        {
            active_devices.push_back(value);
        }
        value->resetDevice();
    }

    currentTime = Time(0, 0);
    orderByStartTime();
    powerLoad = 0;
    log.addLog(report::message(currentTime, "L'orario attuale è stato resettato"));
    checkSchedule();
    log.addLog(report::message(currentTime, "L'orario attuale è " + currentTime.getHourString() + ":" + currentTime.getMinuteString()));
}

void DomoticsSystem::resetTimers()
{ // reset all the timers of the devices
    for (auto *value : active_devices)
    {
        if (ManualDevice *manualDevice = dynamic_cast<ManualDevice *>(value))
        {
            manualDevice->removeStopTime(); // remove the stop time of the manual device
        }
        if (switchedDevices.end() == std::find(switchedDevices.begin(), switchedDevices.end(), value))
        {
            switchedDevices.push_back(value); // add the device to the switched devices if it is not already in the switched devices
        }
    }
    log.addLog(report::message(currentTime, "Tutti i timer dei dispositivi sono stati resettati"));
    checkSchedule(); // check the schedule of the devices
}

void DomoticsSystem::resetAll()
{ // reset all the settings in the system
    currentTime = Time(0, 0);
    for (auto *value : all_devices)
    {
        value->resetDeviceAndTimer(); // reset all the devices
    }
    switchedDevices.clear();
    active_devices.clear();
    powerLoad = 0;
    log.addLog(report::message(currentTime, "Il sistema è stato resettato"));
    checkSchedule();
    log.addLog(report::message(currentTime, "L'orario attuale è " + currentTime.getHourString() + ":" + currentTime.getMinuteString()));
}

void DomoticsSystem::endDay()
{ // turn off all the devices
    currentTime = Time(23, 59);
    for (auto *value : active_devices)
    {
        value->switch_off(currentTime);
    }
    powerLoad = 0;
}