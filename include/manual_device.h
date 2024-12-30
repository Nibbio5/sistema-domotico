#ifndef MANUALDEVICE_H
#define MANUALDEVICE_H

#include "device.h"
#include <string>

class ManulDevice : public Device{
    private:
        int startTime;
        int stopTime;
    
    public:
        ManulDevice(std::string, double, int, int);
        
        ~ManulDevice();

        int getStartTime();

        void setStartTime();

        int getStopTime();
        
        void setStopTime();
};

#endif