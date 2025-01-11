// Eros Menin

#ifndef CPDEVICE_H
#define CPDEVICE_H

#include "device.h"

/**
 * @brief Subclass of Device used to define a domotic device which has a duration that
 * defines for how much time the device stays active before shutting off (after it has been
 * turned on).
 * 
 * The device has to be manually turned on by the user or automatically (by the system) 
 * according to the start time.
 * 
 */
class CPDevice: public Device {
  public:
    /**
     * @brief The amount of time the devices has to work before shutting off
     *
     */
    const Time KDuration;

        /**
         * @brief Construct a new CPDevice object
         * 
         * @param name the name of the device
         * @param power the power consumption (negative) or production (positive) of the device
         * @param duration the amount of time the devices has to work before shutting off
         * @param start_time the time point at which the device turns on
         * @param is_on_white_list whether the device is part of a white list of devices that won't 
         * be turned off in case of electrical overload (if possible).
         */
        CPDevice(const std::string& name, const double power, const Time& duration, const Time& start_time, const bool is_on_white_list = false);

        /**
         * @brief Construct a new CPDevice object without a start time
         * 
         * @param name the name of the device
         * @param power the power consumption (negative) or production (positive) of the device
         * @param duration the amount of time the devices has to work before shutting off
         * @param is_on_white_list whether the device is part of a white list of devices that won't 
         * be turned off in case of electrical overload (if possible).
         */
        CPDevice(const std::string& name, const double power, const Time& duration, const bool is_on_white_list = false);
    

    private:
        /**
         * @brief Output the CPDevice object to the output stream.
         *
         * The data injected in the ostream object contains the class name and a list of
         * all the class variables with the respective value.
         * If the dynamic pointer to the Time object holds a nullptr, the start time
         * is "NOT_SET". 
         *
         * @param out the stream where the data are injected
         * @param device the CPDevice object which provides the data to inject
         * @return std::ostream&: the given output stream
         */
        friend std::ostream& operator<<(std::ostream& out, const CPDevice& device);
};

#endif