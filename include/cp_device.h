// Eros Menin

#ifndef CPDEVICE_H
#define CPDEVICE_H

#include "device.h"
#include "time.h"
#include <ostream>

/**
 * @brief Subclass of Device used to define a domotic device which has a duration that
 * defines for how much time the device stays active before shutting off.
 * The device is manually turned on by the user or automatically according to the start time.
 * 
 */
class CPDevice: public Device{
    public:
        /**
         * @brief The amount of time the devices has to work before shutting off
         * 
         */
        const Time duration;

        /**
         * @brief Construct a new CPDevice object
         * 
         * @param name the name of the device
         * @param power the power consumption (negative) or production (positive) of the device
         * @param startTime the time point at which the devices turns on
         * @param duration the amount of time the devices has to work before shutting off
         */
        CPDevice(std::string name, double power, Time startTime, Time duration);

        /**
         * @brief Construct a new CPDevice object without a start time
         * 
         * @param name the name of the device
         * @param power the power consumption (negative) or production (positive) of the device
         * @param duration the amount of time the devices has to work before shutting off
         */
        CPDevice(std::string name, double power, Time duration);

        /**
         * @brief Destroy the CPDevice object
         * 
         */
        ~CPDevice() = default;

        /**
         * @brief Set the start Time object
         * 
         * @param newTime the new time for the start Time object
         */
        void setStartTime(Time newTime);
};

/**
 * @brief Output the CPDevice object to the output stream.
 * 
 * The data injected in the ostream object contains the class name and a list of all the
 * class variables with the respective value.
 * 
 * @param out the stream where the data are injected
 * @param device the CPDevice object which provides the data to inject
 * @return std::ostream& the given output stream
 */
std::ostream& operator<<(std::ostream& out, const CPDevice& device);

#endif