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
    /*
    * private variables that must be touched  only by the class methods
    * std::vector<T> all_devices;
    * std::vector<T> active_devices;
    * std::double power_limit:
    *
    *  std::vector<T> getDevices();
    */
    std::vector<Device*> all_devices;
    std::vector<Device*> active_devices;
    const double KPowerLimit;
    double powerLoad;
    Time currentTime;

    std::vector<Device*> setDevices();
  public:

    DomoticsSystem();
    //void start(/*${DEVICENAME}*/ );
    //void stop(/*${DEVICENAME}*/);
    // void removeTimer(/*${DEVICENAME}*/);
    std::vector<Device*> getDevices() const;
    void showDevices() const; //
    Time getCurrentTime() const;
    int getIndex(std::string device) const;
    double getPowerLoad() const;
    void setCurrentTime(const Time& newTime);
    void setDeviceTime(const std::string& device, const Time& start, const Time& stop);
    void startDevices(); //
    void changeDeviceStatus(bool status, std::string device);
    void removeDevice(std::string device);
    void balancePower(std::string last);
    void showActiveDevices() const; //
    void currentMod();  //

    //DEBUG section
    void resetTime();
    void resetTimers();
    void resetAll();

    friend std::ostream &operator<<(std::ostream &out, const DomoticsSystem &sys);
};