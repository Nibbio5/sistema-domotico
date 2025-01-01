#ifndef MANUALDEVICE_H
#define MANUALDEVICE_H

#include "device.h"
#include "time.h"
#include <string>
#include <ostream>

class ManualDevice : public Device{
    public:
        ManualDevice(std::string, double, Time, Time);
        
        ~ManualDevice() = default;

        Time getStartTime() const {return startTime;}

        void setStartTime(Time newTime) {startTime = newTime;}

        Time getStopTime() const {return stopTime;}
        
        void setStopTime(Time newTime) {stopTime = newTime;}

        // Works only if the time is within a day
        Time getActivityDuration() const;

        // Works only if the time is within a day
        int getActivityDurationInMinutes() const;

    private:
        Time startTime;
        Time stopTime;
};

std::ostream& operator<<(std::ostream&, const ManualDevice);

#endif