
//template <typename T>
#include <string>  // Aggiungi questo include
#include <vector>
#include <iostream>

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
        std::string devices;

    public:

    DomoticsSystem();
    //void start(/*${DEVICENAME}*/ );
    //void stop(/*${DEVICENAME}*/);
   // void removeTimer(/*${DEVICENAME}*/);

    void setDevices();
    std::string getDevices() const;
    void showDevices() const;
};




/*
*   
*/