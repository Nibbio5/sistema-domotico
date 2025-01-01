
//template <typename T>
#include <string>  // Aggiungi questo include
#include <vector>
#include <iostream>
#include "../include/device.h"
#include "../include/manual_device.h"  // Add this include
#include "../include/cp_device.h"  // Add this include

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
        //std::string devices;

    public:

    DomoticsSystem();
    //void start(/*${DEVICENAME}*/ );
    //void stop(/*${DEVICENAME}*/);
   // void removeTimer(/*${DEVICENAME}*/);
    
     std::vector<Device*> setDevices();
    std::string getDevices() const;
    void showDevices() const;

};