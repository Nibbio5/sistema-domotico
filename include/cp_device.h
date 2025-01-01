#ifndef CPDEVICE_H
#define CPDEVICE_H

#include "device.h"
#include "time.h"
#include <ostream>

class CPDevice: public Device{
    public:
        const Time duration;

        CPDevice(std::string, double, Time);

        ~CPDevice() = default;
};

std::ostream& operator<<(std::ostream&, const CPDevice&);

#endif