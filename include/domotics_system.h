// Daniele Piccolo

#ifndef DOMOTICS_SYSTEM_H
#define DOMOTICS_SYSTEM_H

#include <string>  // Aggiungi questo include
#include <map>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "device.h"
#include "time.h"
#include "manual_device.h"
#include "cp_device.h"
#include "log.h"

/**
 * @brief  A class that manages the domotics system by controlling the devices
 *
 */
class DomoticsSystem {
    private:
    
        std::vector<Device*> all_devices;
        std::vector<Device*> active_devices;
        const double KPowerLimit;
        double powerLoad;
        Time currentTime;

        void checkSchedule();
        void setDevices();
        void resetDevicesPowerCounter(); // to be implemented
        void insertByTime(std::string device);

    public:

    DomoticsSystem();
    DomoticsSystem(double power);
    std::vector<Device*> getDevices() const;
    std::vector<Device *> getDevicesVector() const;
    void showDevices() const; //
    Time getCurrentTime() const; 
    int getIndex(std::string device, bool prova) const;
    bool isPresent(std::string device) const;
    double getPowerLoad() const; 
    void setCurrentTime(const Time& newTime);
    void setDeviceTime(const std::string& device, const Time& start, const Time& stop);
    void changeDeviceStatus (bool status, std::string device); 
    void removeDeviceTimer(std::string device);
    void balancePower(std::string last);
    void showActiveDevices() const; 

    //DEBUG section
    void resetTime();
    void resetTimers();
    void resetAll();

    friend std::ostream &operator<<(std::ostream &out, const DomoticsSystem &sys);
};


#endif // #ifndef