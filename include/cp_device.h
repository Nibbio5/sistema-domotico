#ifndef CPDEVICE_H
#define CPDEVICE_H

#include "device.h"
#include <chrono>
#include <ostream>

class CPDevice: public Device{
    public:
        const std::chrono::seconds duration;

        CPDevice(std::string, double, std::chrono::seconds duration);

        ~CPDevice() = default;
};

std::ostream& operator<<(std::ostream&, const CPDevice&);

#endif