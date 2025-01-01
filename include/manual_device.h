#ifndef MANUALDEVICE_H
#define MANUALDEVICE_H

#include "device.h"
#include <string>
#include <chrono>
#include <ostream>

class ManualDevice : public Device{
    public:
        ManualDevice(std::string, double, std::chrono::system_clock::time_point, std::chrono::system_clock::time_point);
        
        ~ManualDevice() = default;

        std::chrono::system_clock::time_point getStartTime() const {return startTime;}

        void setStartTime(std::chrono::system_clock::time_point newTime) {startTime = newTime;}

        std::chrono::system_clock::time_point getStopTime() const {return stopTime;}
        
        void setStopTime(std::chrono::system_clock::time_point newTime) {stopTime = newTime;}

        std::chrono::seconds getActivityDurationInSeconds() {return std::chrono::duration_cast<std::chrono::seconds>(stopTime - startTime);}

    private:
        std::chrono::system_clock::time_point startTime;
        std::chrono::system_clock::time_point stopTime;
};

std::ostream& operator<<(std::ostream&, const ManualDevice);

#endif