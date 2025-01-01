#ifndef DEVICE_H
#define DEVICE_H

#include <string>
#include <ostream>

class Device{
    public: 
        const int id;
        const std::string name;
        const double power;

        virtual ~Device() = default;

        int isDeviceOn() const {return isOn;}

        void switchOn() {isOn = true;}

        void switchOff() {isOn = false;}

    protected:
        bool isOn;

        Device(std::string, double);

        // Copy constructor and assignment operator disabled
        Device(const Device&) = delete;
        
        Device& operator=(const Device&) = delete;
};

std::ostream& operator<<(std::ostream&, const Device&);

#endif