
//template <typename T>
#include "device.h"
#include <string>  // Aggiungi questo include
#include <map>
#include <iostream>
#include "../include/device.h"
#include "../include/manual_device.h"  // Add this include
#include "../include/cp_device.h"  // Add this include
#include "../include/time.h"  // Add this include
#include <string> // Aggiungi questo include
#include <fstream>
#include <sstream>


/**
 * @brief  A class that manages the domotics system by controlling the devices
 * 
 */
class DomoticsSystem {
    private:
    std::vector<Device*> devices;
        /*
        * private variables that must be touched  only by the class methods
        * std::vector<T> all_devices;
        * std::vector<T> active_devices;
        * std::double power_limit:
        * 
        *  std::vector<T> getDevices();
        */
        std::map<std::string, Device*> all_devices;
        std::map<std::string, Device*> active_devices;
        const double KPowerLimit;
        double powerLoad;
        Time currentTime;

        std::map<std::string, Device*> setDevices();
    public:

    DomoticsSystem();
    //void start(/*${DEVICENAME}*/ );
    //void stop(/*${DEVICENAME}*/);
   // void removeTimer(/*${DEVICENAME}*/);
    std::map<std::string, Device *> getDevices() const;
    void showDevices() const; //
    Time getCurrentTime() const; 
    double getPowerLoad() const; 
    void setCurrentTime(const Time& newTime);
    void setDeviceTime(const std::string& device, const Time& start, const Time& stop);
    void startDevices(); //
    void changeDeviceStatus (bool status, std::string device); 
    void removeDevice(std::string device);
    void balancePower();
    void showActiveDevices() const; //
    void currentMod (); //

    //DEBUG section
    void resetTime();
    void resetTimers();
    void resetAll();
};

