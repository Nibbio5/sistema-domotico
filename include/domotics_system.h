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
    std::map<std::string, Device*, std::less<>> all_devices;
    std::map<std::string, Device*, std::less<>> active_devices;
    const double KPowerLimit;
    double powerLoad;
    Time currentTime;

    std::map<std::string, Device*, std::less<>> setDevices();
  public:

    DomoticsSystem();
    //void start(/*${DEVICENAME}*/ );
    //void stop(/*${DEVICENAME}*/);
    // void removeTimer(/*${DEVICENAME}*/);
    std::map<std::string, Device*, std::less<>> getDevices() const;
    std::vector<Device *> getDevicesVector() const;
    void showDevices() const; //
    Time getCurrentTime() const;
    double getPowerLoad() const;
    void setCurrentTime(const Time& newTime);
    void setDeviceTime(const std::string& device, const Time& start, const Time& stop);
    void startDevices(); //
    void changeDeviceStatus(bool status, const std::string& device);
    void removeDevice(const std::string &device);
    void balancePower();
    void showActiveDevices() const; //
    void currentMod();  //

    //DEBUG section
    void resetTime();
    void resetTimers();
    void resetAll();

    friend std::ostream &operator<<(std::ostream &out, const DomoticsSystem &sys);
};